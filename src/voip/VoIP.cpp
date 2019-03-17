#include "VoIP.h"
#include <pthread.h>
#include "../media/Media.h"
#include "timer.h"

/*
 * @function:
 * @brief: class constructor, initialize gstreamer library
 * @parameters: null
 * @return: null
 */
VoIP::VoIP() : m_store_data(false)
{
    gst_init(NULL, NULL);
}

/*
 * @function:
 * @brief: class destructor, stop gstreamer pipeline
 * @parameters: null
 * @return: null
 */
VoIP::~VoIP()
{
    stop_pipeline();
}

/*
 * @function:
 * @brief: 
 * @parameters: 
 * @return: 
 */
void VoIP::create_pipeline(int port)
{
    auto pipeline = "udpsrc port=" + std::to_string(port);
    pipeline += " ! application/x-rtp,media=(string)audio, clock-rate=(int)44100,";
    pipeline += " width=16, height=16, encoding-name=(string)L16, encoding-params=(string)1,";
    pipeline += " channels=(int)1, channel-positions=(int)1, payload=(int)96 ! rtpL16depay !";
    pipeline += " audioconvert ! audioresample ! audioconvert !";
    pipeline += " audio/x-raw,format=S16LE,rate=44100,channels=1 ! appsink name=appsink1";
    g_print("Starting pipeline: %s\n", pipeline.c_str());
    m_pipeline = gst_parse_launch(pipeline.c_str(), NULL);
}

/*
 * @function:
 * @brief: 
 * @parameters: 
 * @return: 
 */
void VoIP::config_pipeline()
{
    // configure for appsink
    auto appsink = gst_bin_get_by_name(GST_BIN(m_pipeline), "appsink1");
    // enable signal
    g_object_set(appsink, "emit-signals", TRUE, NULL);
    // connect signal to a callback
    g_signal_connect(appsink, "new-sample", G_CALLBACK(new_sample), this);
    gst_object_unref(appsink);

    // emmit signals for each error message
    auto bus = gst_element_get_bus(m_pipeline);
    gst_bus_add_signal_watch(bus);
    g_signal_connect(G_OBJECT(bus), "message::error", (GCallback)error_cb, this);
}

/*
 * @function:
 * @brief: 
 * @parameters: 
 * @return: 
 */
GstFlowReturn VoIP::new_sample(GstElement *sink, void *data)
{
    GstSample *sample;

    auto obj = reinterpret_cast<VoIP *>(data);

    if (obj->m_store_data)
    {
        // Retrieve the buffer
        g_signal_emit_by_name(sink, "pull-sample", &sample);
        if (sample)
        {
            // TODO: store 3s to file
            auto buffer = gst_sample_get_buffer(sample);
            GstMapInfo map;
            gst_buffer_map(buffer, &map, GST_MAP_READ);

            Media::write_pcm(obj->get_file_name(), (char *)map.data, map.size);

            if (map.size > 0)
            {
                gst_buffer_unmap(buffer, &map);
            }

            if (gst_buffer_get_size(buffer) > 0)
            {
                //gst_buffer_unref(buffer);
            }
            gst_sample_unref(sample);
            return GST_FLOW_OK;
        }
        return GST_FLOW_ERROR;
    }

    return GST_FLOW_OK;
}

/*
 * @function:
 * @brief: 
 * @parameters: 
 * @return: 
 */
void VoIP::error_cb(GstBus *bus, GstMessage *msg, void *data)
{
    GError *err;
    gchar *debug_info;
    auto obj = reinterpret_cast<VoIP *>(data);
    // print error details on the screen
    gst_message_parse_error(msg, &err, &debug_info);
    g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
    g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
    g_clear_error(&err);
    g_free(debug_info);
    g_main_loop_quit(obj->m_main_loop);
}

/*
 * @function:
 * @brief: 
 * @parameters: 
 * @return: 
 */
void VoIP::start_pipeline(int port)
{
    create_pipeline(port);
    config_pipeline();
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, &voip_thread, this);
}

/*
 * @function:
 * @brief: 
 * @parameters: 
 * @return: 
 */
void *VoIP::voip_thread(void *data)
{
    auto obj = reinterpret_cast<VoIP *>(data);
    obj->m_main_loop = g_main_loop_new(NULL, FALSE);
    gst_element_set_state(obj->m_pipeline, GST_STATE_PLAYING);
    g_main_loop_run(obj->m_main_loop);
}

/*
 * @function:
 * @brief: 
 * @parameters: 
 * @return: 
 */
void VoIP::stop_pipeline()
{
    gst_element_set_state(m_pipeline, GST_STATE_NULL);
    g_main_loop_quit(m_main_loop);
    g_main_loop_unref(m_main_loop);
    gst_object_unref(m_pipeline);
}

/*
 * @function:
 * @brief: 
 * @parameters: 
 * @return: 
 */
void VoIP::toggle_test()
{
    set_file_name(Media::get_file_name(std::to_string(Timer::get_time_stamp()), Media::PCM));
    m_store_data = true;

    // start timer
    auto t = Timer();
    t.setTimeout([&]() {
        m_store_data = false;
        Media::pcm_to_flac(m_current_file,
                           Media::change_file_extension(m_current_file, Media::FLAC));
        t.stop();
    },5000); // Timer out 5s
}