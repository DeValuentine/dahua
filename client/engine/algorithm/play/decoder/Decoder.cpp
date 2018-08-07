#include "Decoder.hpp"

#define REAL_AV_CODEC_ID_H264 (AVCodecID)28
#define ONE_MORE_REAL_AV_CODEC_ID_H264 (AVCodecID)( (int)AV_CODEC_ID_H264+1 )

void dahua::algorithm::process::playutils::Translator::init() {
    if (m_inited)
        return;
    av_register_all();
//    vLOG("%s[%i]", avcodec_get_name(AV_CODEC_ID_H264), (int)AV_CODEC_ID_H264);
    m_codec = avcodec_find_decoder( REAL_AV_CODEC_ID_H264 );
    if (!m_codec) { 
        vLOG("[Translator] (Error): Codec(H264) not found\n");
        std::terminate();
    }
    m_ctx = avcodec_alloc_context3(m_codec);
    if (!m_ctx) {
        vLOG("[Translator] (Error): Could not alloc avCodecContext\n");
        std::terminate();
    }
//    av_register_codec_parser(m_codec_parser);
   
    if (m_codec->capabilities&AV_CODEC_CAP_TRUNCATED) {
        m_ctx->flags |= AV_CODEC_FLAG_TRUNCATED;
    }
    AVDictionary* dict = NULL;
    if (avcodec_open2(m_ctx, m_codec, &dict) < 0) {
        vLOG("[Translator] (Error): Could not open codec\n");
        std::terminate();
    }
//    for (int i = 0; i < 500; i++) {
//        if (av_parser_init( (AVCodecID)i ) != nullptr)
//            vLOG("Parser is not NULL. av_codec_id = %s[%i]\n", avcodec_get_name((AVCodecID)i), i);
//    }
    m_parser = av_parser_init( REAL_AV_CODEC_ID_H264 );
    if (!m_parser) {
        vLOG("[Translator] (Error): Could not alloc avCodecParserContext\n");
        std::terminate();
    }
    m_parser->flags |= PARSER_FLAG_ONCE;
    
    if (av_bsf_get_null_filter(&m_bsf_ctx) < 0) {
        vLOG("[Translator] (Error): Could not alloc bsf\n");
        std::terminate();
    }
    av_bsf_init(m_bsf_ctx);
    
    m_pkt_recieved = av_packet_alloc();
    if (!m_pkt_recieved) {
        vLOG("[Translator] (Error): Could not alloc avPacket\n");
        std::terminate();
    }
    m_pkt_parsed = av_packet_alloc();
    if (!m_pkt_parsed) {
        vLOG("[Translator] (Error): Could not alloc avPacket\n");
        std::terminate();
    }
    m_inited = true;
    vLOG("[Translator] -> Inited\n");
}

void dahua::algorithm::process::playutils::Translator::deinit() {
    if (!m_inited)
        return;
    av_packet_free(&m_pkt_recieved);
    avcodec_close(m_ctx);
    av_parser_close(m_parser);
    av_bsf_free(&m_bsf_ctx);
    avcodec_free_context(&m_ctx);
    m_inited = false;
    
    vLOG("[Translator] -> Deinited\n");
}

void dahua::algorithm::process::playutils::Translator::update(dahua::algorithm::h264::Raw& _data) {
    if (!m_inited)
        return;
    decode_package(_data);
    notify();
}
#define ADRESS_OF( name, var ) vLOG("AdressOF: %s[%p]\n", name, var);
inline void dahua::algorithm::process::playutils::Translator::decode_package(dahua::algorithm::h264::Raw const& _raw) {
//    ADRESS_OF("_raw.data", _raw.data); 
    if (!_raw.size || !_raw.data) {
        return;
    }
//    ADRESS_OF("data", data);
    if (av_packet_from_data(m_pkt_recieved, _raw.data, _raw.size) < 0)
        return;
    av_bsf_send_packet(m_bsf_ctx, m_pkt_recieved);
    av_packet_unref(m_pkt_recieved);
    
    while ( !av_bsf_receive_packet(m_bsf_ctx, m_pkt_recieved) ) {
        while (m_pkt_recieved->size > 0) {
            int res_len;
            int pts, dts;
            res_len = av_parser_parse2(m_parser, m_ctx, &m_pkt_parsed->data, &m_pkt_parsed->size, m_pkt_recieved->data, m_pkt_recieved->size, pts, dts, AV_NOPTS_VALUE);
            if (res_len < 0) {
                vLOG("[Translator] (Error): Unparsed frame[%d]\n", m_frame_count);
                av_packet_unref(m_pkt_parsed);
                av_packet_unref(m_pkt_recieved);
                return;
            }            
            if (m_pkt_parsed->size > 0) {
                int is_frame = 0;
                AVFrame* avframe = av_frame_alloc();
                res_len = avcodec_decode_video2(m_ctx, avframe, &is_frame, m_pkt_parsed);
                if (res_len < 0) {
                    vLOG("[Translator] (Error): Undecoded frame[%d]\n", m_frame_count);
                    av_packet_unref(m_pkt_parsed);
                    av_packet_unref(m_pkt_recieved);
                    return;
                }
                // post-processing //
                if (is_frame) {
                    m_frame_count++;
                    m_mutex.lock();
                    m_frames.push(avframe);
                    m_mutex.unlock();
                }
            }
            av_packet_unref(m_pkt_parsed);
            if (m_pkt_recieved->size > 0) {
                m_pkt_recieved->size -= res_len;
                m_pkt_recieved->data += res_len;
            }
        }
        av_packet_unref(m_pkt_recieved);
//        av_free(copied_data);
    } 
}

inline void dahua::algorithm::process::playutils::Translator::convertToMat(AVFrame* frame, cv::Mat& image) {
    // Allocate the opencv mat and store its stride in a 1-element array
    if (image.rows != m_frame_size.height || image.cols != m_frame_size.width || image.type() != CV_8UC3) 
        image = cv::Mat(m_frame_size.height, m_frame_size.width, CV_8UC3);
    int cvLinesizes[1];
    cvLinesizes[0] = image.step1();

    // Convert the colour format and write directly to the opencv matrix
    SwsContext* conversion = sws_getContext(m_frame_size.width, m_frame_size.height, (AVPixelFormat) frame->format, m_frame_size.width, m_frame_size.height, AV_PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
    sws_scale(conversion, frame->data, frame->linesize, 0, m_frame_size.height, &image.data, cvLinesizes);
    sws_freeContext(conversion);
}

void dahua::algorithm::process::playutils::Translator::setToCurrent(cv::Mat& cv_frame) {
    m_mutex.lock();
    AVFrame* av_frame = m_frames.front();
    m_frames.pop();
    m_mutex.unlock();
    convertToMat(av_frame, cv_frame);
    av_frame_free(&av_frame);
    return;
}

//dahua::algorithm::process::playutils::FrameBuffer::Iterator dahua::algorithm::process::playutils::Translator::createIterator() {
//    std::lock_guard<std::mutex> lock(m_mutex);
//    return m_buffer.begin();
//}