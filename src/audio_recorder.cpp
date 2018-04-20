#include "audio_recorder.h"

AudioRecorder::AudioRecorder(QString record_url, QObject *parent) : QObject(parent)
{
    this->record_url = record_url;
}

void AudioRecorder::recStart()
{
    GstElement *source, *muxer, *encoder, *conv, *filesink;
    GError **gerror;
    
    int *argc = 0;
    char **argv[0] = {};
    gboolean gst_init_success = gst_init_check(argc, argv, gerror);
    
    pipeline = gst_pipeline_new("audio-recorder");
    source = gst_element_factory_make("autoaudiosrc", "source");
    conv = gst_element_factory_make("audioconvert", "converter");
    encoder = gst_element_factory_make("vorbisenc", "vorbisenc");
    muxer = gst_element_factory_make("oggmux", "oggmux");
    filesink = gst_element_factory_make("filesink", "filesink");
    
    QByteArray q_url_arr = this->record_url.toLatin1();
    gchar *record_url = q_url_arr.data();
    g_object_set(G_OBJECT(filesink), "location", record_url, NULL);
    
    gst_bin_add_many(GST_BIN(pipeline), source, conv, encoder, muxer, filesink, NULL);
    
    gst_element_link(source, conv);
    gst_element_link(conv, encoder);
    gst_element_link(encoder, muxer);
    gst_element_link(muxer, filesink);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

void AudioRecorder::recStop()
{
    gst_element_set_state(pipeline, GST_STATE_NULL);
}
