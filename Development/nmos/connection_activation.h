#ifndef NMOS_CONNECTION_ACTIVATION_H
#define NMOS_CONNECTION_ACTIVATION_H

#include <functional>

namespace web
{
    namespace json
    {
        class value;
    }
}

namespace slog
{
    class base_gate;
}

namespace nmos
{
    struct node_model;
    struct resource;

    // a connection_resource_auto_resolver overwrites every instance of "auto" in the specified transport_params array for the specified (IS-04/IS-05) sender/connection_sender or receiver/connection_receiver
    // it may throw e.g. web::json::json_exception or std::runtime_error, which will prevent activation and for immediate activations cause a 500 Internal Error status code to be returned
    typedef std::function<void(const nmos::resource& node_resource, const nmos::resource& connection_resource, web::json::value& transport_params)> connection_resource_auto_resolver;

    // a connection_sender_transportfile_setter updates the specified /transportfile endpoint for the specified (IS-04/IS-05) sender/connection_sender
    // it may throw e.g. web::json::json_exception or std::runtime_error, which will prevent activation and for immediate activations cause a 500 Internal Error status code to be returned
    typedef std::function<void(const nmos::resource& node_sender, const nmos::resource& connection_sender, web::json::value& endpoint_transportfile)> connection_sender_transportfile_setter;

    // a connection_activation_handler is a notification that the active parameters for the specified (IS-04/IS-05) sender/connection_sender or receiver/connection_receiver have changed
    // for now, this callback must not throw exceptions
    typedef std::function<void(const nmos::resource& node_resource, const nmos::resource& connection_resource)> connection_activation_handler;

    // callbacks from this function are called with the model locked, and may read but should not write directly to the model
    void connection_activation_thread(nmos::node_model& model, connection_resource_auto_resolver resolve_auto, connection_sender_transportfile_setter set_transportfile, connection_activation_handler connection_activated, slog::base_gate& gate);
}

#endif
