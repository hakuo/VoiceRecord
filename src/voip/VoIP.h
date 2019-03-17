#pragma once
#include <gst/gst.h>
#include <glib.h>
#include <string>

class VoIP
{
  private:
    GstElement *m_pipeline;
    GMainLoop *m_main_loop;
    volatile bool m_store_data;
    std::string m_current_file;
    void create_pipeline(int port);
    void config_pipeline();

    static void error_cb(GstBus *bus, GstMessage *msg, void *data);
    static GstFlowReturn new_sample(GstElement *sink, void *data);
    static void *voip_thread(void *data);

  public:
    VoIP();
    ~VoIP();
    inline void set_file_name(std::string _filename) { this->m_current_file = _filename; }
    inline std::string get_file_name() { return this->m_current_file; }
    void start_pipeline(int port);
    void stop_pipeline();
    static int get_time_stamp();
    void toggle_test();
};
