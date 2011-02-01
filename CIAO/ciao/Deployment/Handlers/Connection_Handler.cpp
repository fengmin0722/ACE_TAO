// $Id$
#include "Connection_Handler.h"
#include "ciao/Logger/Log_Macros.h"
#include "dance/Deployment/Deployment_InvalidConnectionC.h"
#include "CIAO_State.h"

namespace CIAO
{
  void
  Connection_Handler::provide_endpoint_reference (const ::Deployment::DeploymentPlan & plan,
                                                  ::CORBA::ULong connectionRef,
                                                  ::CORBA::Any_out endpoint_reference)
  {
    CIAO_TRACE ("Component_Handler_i::provide_endpoint_reference");

    const ::Deployment::PlanConnectionDescription &conn =
      plan.connection[connectionRef];

    try
      {
        CIAO_DEBUG (9, (LM_TRACE, CLINFO
                        "Connection_Handler::provide_endpoint_reference - "
                        "Called for connection <%C>\n",
                        conn.name.in ()));

        for (CORBA::ULong i = 0;
             i < conn.internalEndpoint.length ();
             ++i)
          {
            if (conn.internalEndpoint[i].provider)
              {
                CORBA::Any *out;
                CORBA::Object_var ref;
                CORBA::ULong instanceRef = conn.internalEndpoint[i].instanceRef;
                Components::CCMObject_var comp =
                  DEPLOYMENT_STATE::instance ()->
                  fetch_component (plan.instance[instanceRef].name.in ());

                if (CORBA::is_nil (comp.in ()))
                  {
                    CIAO_ERROR (1, (LM_ERROR, CLINFO
                                    "Connection_Handler::provide_endpoint_reference - "
                                    "Nil component reference from CIAO_State for <%C>\n",
                                    plan.instance[instanceRef].name.in ()));
                    throw ::Deployment::InvalidConnection (conn.name.in (),
                                                           "Component instance not deployed when "
                                                           "gathering connections.");
                  }

                ACE_NEW_THROW_EX (out,
                                  CORBA::Any (),
                                  CORBA::NO_MEMORY ());

                endpoint_reference = out;

                if (this->is_local_connection (conn))
                  (*out) <<= comp.in ();
                else
                  {
                    switch (conn.internalEndpoint[i].kind)
                      {
                      case ::Deployment::Facet:
                        ref =
                          comp->provide_facet (conn.internalEndpoint[i].portName.in ());
                        (*out) <<= ref.in ();
                        break;
#if !defined (CCM_NOEVENT)
                      case ::Deployment::EventConsumer:
                        ref =
                          comp->get_consumer (conn.internalEndpoint[i].portName.in ());
                        (*out) <<= ref.in ();
                        break;
#endif
                      default:
                        throw ::Deployment::InvalidConnection (conn.name.in (),
                                                               "Invalid provider port type.");
                      };
                  }
                return;
              }
          }
      }
    catch (const ::Deployment::InvalidConnection &)
      {
        // pass through
        throw;
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::provide_endpoint_reference - "
                        "Caught CORBA Exception <%C> while fetching reference for "
                        "connection <%C>\n",
                        ex._info ().c_str (),
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               ex._info ().c_str ());

      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::provide_endpoint_reference - "
                        "Caught unknown C++ exception on instance %C",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Unknown C++ exception whilst establishing "
                                               "connection");
      }

  }

  void
  Connection_Handler::connect_instance (const ::Deployment::DeploymentPlan & plan,
                                        ::CORBA::ULong c_id,
                                        const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Connection_Handler::connect_instance");

    const ::Deployment::PlanConnectionDescription &conn = plan.connection[c_id];

    CORBA::ULong endpoint = this->retrieve_endpoint (conn);

    try
      {
        switch (conn.internalEndpoint[endpoint].kind)
          {
          case Deployment::Facet:
            this->connect_facet (plan, c_id, endpoint, provided_reference);
            break;

          case Deployment::SimplexReceptacle:
          case Deployment::MultiplexReceptacle:
            this->connect_receptacle (plan, c_id, endpoint, provided_reference);
            break;
#if !defined (CCM_NOEVENT)
          case Deployment::EventEmitter:
            this->connect_emitter (plan, c_id, endpoint, provided_reference);
            break;

          case Deployment::EventPublisher:
            this->connect_publisher (plan, c_id, endpoint, provided_reference);
            break;
#endif
          default:
            CIAO_ERROR (1, (LM_ERROR, CLINFO
                            "Connection_Handler::connect_instance - "
                            "Unsupported port type.\n"));
            throw ::Deployment::InvalidConnection (conn.name.in (),
                                                   "Unsupported port type");

          }
      }
    catch (const ::Deployment::InvalidConnection &)
      {
        // pass through
        throw;
      }
    catch (CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_instance - "
                        "Caught CORBA exception whilst connecting <%C>: %C\n",
                        conn.name.in (),
                        ex._info ().c_str ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_instance - "
                        "Caught C++ exception whilst connecting <%C>\n",
                        conn.name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               "Unknown C++ Exception");
      }
  }

  void
  Connection_Handler::disconnect_instance (const ::Deployment::DeploymentPlan &plan,
                                           ::CORBA::ULong c_id)
  {
    CIAO_TRACE ("Connection_Handler::disconnect_instance");

    const ::Deployment::PlanConnectionDescription &conn = plan.connection[c_id];

    CORBA::ULong endpoint = this->retrieve_endpoint (conn);

    try
      {
        switch (conn.internalEndpoint[endpoint].kind)
          {
          case Deployment::Facet:
            this->disconnect_facet (plan, c_id, endpoint);
            break;

          case Deployment::SimplexReceptacle:
          case Deployment::MultiplexReceptacle:
            this->disconnect_receptacle (plan, c_id, endpoint);
            break;
#if !defined (CCM_NOEVENT)
          case Deployment::EventEmitter:
            this->disconnect_emitter (plan, c_id, endpoint);
            break;

          case Deployment::EventPublisher:
            this->disconnect_publisher (plan, c_id, endpoint);
            break;
#endif
          default:
            CIAO_ERROR (1, (LM_ERROR, CLINFO
                            "Connection_Handler::disconnect_instance - "
                            "Unsupported port type.\n"));
            throw ::Deployment::InvalidConnection (conn.name.in (),
                                                   "Unsupported port type");

          }
      }
    catch (const ::Deployment::InvalidConnection &)
      {
        // pass through
        throw;
      }
    // Since DANCE shutdown the Locality managers simultaniously,
    // it could be that one locality manager is shutdown while the
    // other wants to disconnect from this locality manager. Therefor
    // we catch an OBJECT_NOT_EXIST, TRANSIENT and a COMM_FAILURE at this point
    // Once DANCE has been fixed in that manner, these catches can be removed.
    catch (const CORBA::COMM_FAILURE &ex)
      {
        CIAO_DEBUG (2, (LM_WARNING, CLINFO
                        "Connection_Handler::disconnect_instance - "
                        "Caught COMM_FAILURE exception whilst disconnecting <%C>\n",
                        conn.name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               ex._info ().c_str ());
      }
    catch (const CORBA::OBJECT_NOT_EXIST &ex)
      {
        CIAO_DEBUG (2, (LM_WARNING, CLINFO
                        "Connection_Handler::disconnect_instance - "
                        "Caught OBJECT_NOT_EXIST exception whilst disconnecting <%C>\n",
                        conn.name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               ex._info ().c_str ());
      }
    catch (const CORBA::TRANSIENT &ex)
      {
        CIAO_DEBUG (2, (LM_WARNING, CLINFO
                        "Connection_Handler::disconnect_instance - "
                        "Caught TRANSIENT exception whilst disconnecting <%C>\n",
                        conn.name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               ex._info ().c_str ());
      }
    catch (CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::disconnect_instance - "
                        "Caught CORBA exception whilst disconnecting <%C>: %C\n",
                        conn.name.in (),
                        ex._info ().c_str ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::disconnect_instance - "
                        "Caught C++ exception whilst disconnecting <%C>\n",
                        conn.name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               "Unknown C++ Exception");
      }
  }

  void
  Connection_Handler::connect_facet (const ::Deployment::DeploymentPlan & plan,
                                     ::CORBA::ULong connectionRef,
                                     ::CORBA::ULong endpointRef,
                                     const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Connection_Handler::connect_facet");

    const ::Deployment::PlanConnectionDescription &conn =
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &endpoint =
      conn.internalEndpoint[endpointRef];

    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Connection_Handler::connect_facet - "
                    "Connecting connection <%C> on instance <%C>\n",
                    conn.name.in (),
                    plan.instance[endpoint.instanceRef].name.in ()));

    if (this->is_local_connection (conn))
      {
        this->connect_local_port (plan, conn, endpointRef, endpoint);
      }
    else
      {
        this->connect_non_local_facet (plan,
                                      connectionRef,
                                      endpointRef,
                                    provided_reference);
      }
  }

  void
  Connection_Handler::connect_non_local_facet (const ::Deployment::DeploymentPlan & plan,
                                               ::CORBA::ULong connectionRef,
                                               ::CORBA::ULong endpointRef,
                                               const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Connection_Handler::connect_non_local_facet");

    const ::Deployment::PlanConnectionDescription &conn =
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &endpoint =
      conn.internalEndpoint[endpointRef];

    if (conn.externalReference.length () == 0)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_non_local_facet - "
                        "Error: Expected external reference endpoint for connection <%C>",
                        conn.name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               "Expected external reference connection.");
      }

    ::CORBA::Object_var obj;
    provided_reference >>= CORBA::Any::to_object (obj);

    ::Components::CCMObject_var provided = ::Components::CCMObject::_narrow (obj.in ());

    if (CORBA::is_nil (provided))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_non_local_facet - "
                        "While connecting <%C>:"
                        "Provided reference for connection where primary is Facet must be "
                        "non-nil.\n",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Provided reference for connection where primary is Facet must "
                                               "be non-nil.");

      }

    ::Components::CCMObject_var facet_provider =
        DEPLOYMENT_STATE::instance ()->fetch_component (plan.instance[endpoint.instanceRef].name.in ());

    if (CORBA::is_nil (facet_provider))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_non_local_facet - "
                        "While connecting <%C>:"
                        "Providing component not deployed.",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Providing component not deployed.");
      }

    CORBA::Object_var facet =
      facet_provider->provide_facet (endpoint.portName.in ());

    ::Components::Cookie_var cookie = provided->connect (conn.externalReference[0].portName.in (),
                                                         facet.in ());
    CIAO_DEBUG (5, (LM_INFO, CLINFO
                    "Connection_Handler::connect_non_local_facet - "
                    "Connection <%C> successfully established.\n",
                    conn.name.in ()));

    CONNECTION_INFO conn_info = CONNECTION_INFO (cookie._retn (),
                                                 ::Components::CCMObject::_duplicate (provided.in ()));
    this->insert_cookie (conn.name.in (), conn_info);
  }

  void
  Connection_Handler::connect_receptacle (const ::Deployment::DeploymentPlan & plan,
                                          ::CORBA::ULong connectionRef,
                                          ::CORBA::ULong endpointRef,
                                          const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Component_Handler_i::connect_receptacle");

    const ::Deployment::PlanConnectionDescription &conn =
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &receptacle_endpoint =
      conn.internalEndpoint[endpointRef];

    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Connection_Handler::connect_receptacle - "
                    "Connecting connection <%C> on instance <%C>\n",
                    conn.name.in (),
                    plan.instance[receptacle_endpoint.instanceRef].name.in ()));

    if (this->is_local_connection (conn))
      {
        this->connect_local_port (plan, conn, endpointRef, receptacle_endpoint);
      }
    else
      {
        this->connect_non_local_receptacle (plan,
                                            connectionRef,
                                            endpointRef,
                                            provided_reference);
      }
  }

  void
  Connection_Handler::connect_non_local_receptacle (const ::Deployment::DeploymentPlan & plan,
                                                    ::CORBA::ULong connectionRef,
                                                    ::CORBA::ULong endpointRef,
                                                    const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Component_Handler_i::connect_non_local_receptacle");

    const ::Deployment::PlanConnectionDescription &conn =
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &receptacle_endpoint =
      conn.internalEndpoint[endpointRef];

    ::CORBA::Object_var provided;

    if (!(provided_reference >>= CORBA::Any::to_object  (provided)))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_non_local_receptacle - "
                        "Unable to extract provided reference to CORBA::Object\n",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Unable to extract provided reference to CORBA Object.");
      }

    if (conn.externalReference.length () != 0)
      {
        if (ACE_OS::strlen (conn.externalReference[0].portName.in ()) > 0)
          {
            ::Components::CCMObject_var facet_provider =
              ::Components::CCMObject::_narrow (provided.in ());

            if (CORBA::is_nil (facet_provider.in ()))
              {
                CIAO_ERROR (1, (LM_ERROR, CLINFO
                                "Connection_Handler::connect_non_local_receptacle - "
                                "Unable to narrow provided external reference "
                                "to CCMObject in connection <%C> for port <%C>\n",
                                conn.name.in (),
                                conn.externalReference[0].portName.in ()));
                throw ::Deployment::InvalidConnection (conn.name.in (),
                                                       "Unable to narrow external reference to CCMObject\n");
              }

            provided =
              facet_provider->provide_facet (conn.externalReference[0].portName.in ());
          }
      }

    ::Components::CCMObject_var receptacle =
        DEPLOYMENT_STATE::instance ()->fetch_component (plan.instance[receptacle_endpoint.instanceRef].name.in ());

    if (CORBA::is_nil (receptacle.in ()))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_non_local_receptacle - "
                        "While connecting <%C>:"
                        "Receptacle component <%C> not deployed.\n",
                        plan.connection[connectionRef].name.in (),
                        plan.instance[receptacle_endpoint.instanceRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Receptacle component not deployed.");
      }

    ::Components::Cookie_var cookie = receptacle->connect (receptacle_endpoint.portName.in (),
                                                           provided.in ());
    CIAO_DEBUG (5, (LM_INFO, CLINFO
                    "Connection_Handler::connect_non_local_receptacle - "
                    "Connection <%C> successfully established.\n",
                    conn.name.in ()));

    CONNECTION_INFO conn_info = CONNECTION_INFO (cookie._retn (),
                                                 ::Components::CCMObject::_duplicate (receptacle.in ()));
    this->insert_cookie (conn.name.in (), conn_info);
  }

#if !defined (CCM_NOEVENT)
  void
  Connection_Handler::connect_publisher (const ::Deployment::DeploymentPlan & plan,
                                         ::CORBA::ULong connectionRef,
                                         ::CORBA::ULong endpointRef,
                                         const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Connection_Handler::connect_publisher");

    const ::Deployment::PlanConnectionDescription &conn =
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &endpoint =
      conn.internalEndpoint[endpointRef];

    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Connection_Handler::connect_publisher - "
                    "Connecting connection <%C> on instance <%C>\n",
                    conn.name.in (),
                    plan.instance[endpoint.instanceRef].name.in ()));

    ::CORBA::Object_var consumer;

    if (!(provided_reference >>= CORBA::Any::to_object  (consumer)) ||
        CORBA::is_nil (consumer.in ()))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_publisher - "
                        "Unable to extract provided reference to CORBA::Object\n",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Unable to extract provided reference to CORBA Object.");
      }

    Components::EventConsumerBase_var event =
      Components::EventConsumerBase::_unchecked_narrow (consumer);

    ::Components::CCMObject_var publisher =
        DEPLOYMENT_STATE::instance ()->fetch_component (plan.instance[endpoint.instanceRef].name.in ());

    if (CORBA::is_nil (publisher))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_publisher - "
                        "While connecting <%C>:"
                        "Publishing component not deployed.",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               "Publisher component not deployed.");
      }
    Components::Cookie_var cookie = publisher->subscribe (endpoint.portName.in (),
                                                          event.in ());

    CIAO_DEBUG (5, (LM_INFO, CLINFO
                    "Connection_Handler::connect_publisher - "
                    "Connection <%C> successfully established.\n",
                    conn.name.in ()));

    CONNECTION_INFO conn_info = CONNECTION_INFO (cookie._retn (),
                                                 ::Components::CCMObject::_duplicate (publisher.in ()));
    this->insert_cookie (conn.name.in (), conn_info);
  }
#endif

#if !defined (CCM_NOEVENT)
  void
  Connection_Handler::connect_emitter (const ::Deployment::DeploymentPlan & plan,
                                       ::CORBA::ULong connectionRef,
                                       ::CORBA::ULong endpointRef,
                                       const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Connection_Handler::connect_emitter");

    const ::Deployment::PlanConnectionDescription &conn =
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &endpoint =
      conn.internalEndpoint[endpointRef];

    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Connection_Handler::connect_emitter - "
                    "Connecting connection <%C> on instance <%C>\n",
                    conn.name.in (),
                    plan.instance[endpoint.instanceRef].name.in ()));

    ::CORBA::Object_var consumer;

    if (!(provided_reference >>= CORBA::Any::to_object  (consumer)) ||
        CORBA::is_nil (consumer.in ()))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_emitter - "
                        "Unable to extract provided reference to CORBA::Object\n",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Unable to extract provided reference to CORBA Object.");
      }

    Components::EventConsumerBase_var event =
      Components::EventConsumerBase::_unchecked_narrow (consumer);

    ::Components::CCMObject_var emitter =
        DEPLOYMENT_STATE::instance ()->fetch_component (plan.instance[endpoint.instanceRef].name.in ());

    if (CORBA::is_nil (emitter))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_emitter - "
                        "While connecting <%C>:"
                        "Emitting component not deployed.",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               "Emitting component not deployed.");
      }

    emitter->connect_consumer (endpoint.portName.in (),
                               event.in ());

    CIAO_DEBUG (5, (LM_INFO, CLINFO
                    "Connection_Handler::connect_emitter - "
                    "Connection <%C> successfully established.\n",
                    conn.name.in ()));

    ::Components::Cookie_var nil_cookie;

    CONNECTION_INFO conn_info = CONNECTION_INFO (nil_cookie._retn (),
                                                 ::Components::CCMObject::_duplicate (emitter.in ()));
    this->insert_cookie (conn.name.in (), conn_info);
  }
#endif

  void
  Connection_Handler::disconnect_facet (const ::Deployment::DeploymentPlan &plan,
                                        ::CORBA::ULong connectionRef,
                                        ::CORBA::ULong endpointRef)

  {
    CIAO_TRACE ("Connection_Handler::disconnect_facet");

    const ::Deployment::PlanConnectionDescription &conn =
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &receptacle_endpoint =
      conn.internalEndpoint[endpointRef];

    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Connection_Handler::disconnect_facet - "
                    "Disconnecting connection <%C> on instance <%C>\n",
                    conn.name.in (),
                    plan.instance[receptacle_endpoint.instanceRef].name.in ()));

    if (this->is_local_connection (conn))
      {
        this->disconnect_local_port (plan, conn, endpointRef, receptacle_endpoint);
      }
    else
      {
        this->disconnect_non_local (conn, receptacle_endpoint);
      }
  }

  void
  Connection_Handler::disconnect_non_local (const ::Deployment::PlanConnectionDescription &conn,
                                            const ::Deployment::PlanSubcomponentPortEndpoint &endpoint)
  {
    CIAO_TRACE ("Connection_Handler::disconnect_non_local");

    ::Components::CCMObject_var obj = this->get_ccm_object (conn.name.in ());
    ::CORBA::Object_var safe_tmp =
      obj->disconnect (endpoint.portName.in (),
                       this->get_cookie (conn.name.in ()));
    this->remove_cookie (conn.name.in ());
  }

  void
  Connection_Handler::disconnect_receptacle (const ::Deployment::DeploymentPlan &plan,
                                             ::CORBA::ULong connectionRef,
                                             ::CORBA::ULong endpointRef)

  {
    CIAO_TRACE ("Connection_Handler::disconnect_receptacle");

    const ::Deployment::PlanConnectionDescription &conn =
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &receptacle_endpoint =
      conn.internalEndpoint[endpointRef];

    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Connection_Handler::disconnect_receptacle - "
                    "Disconnecting connection <%C> on instance <%C>\n",
                    conn.name.in (),
                    plan.instance[receptacle_endpoint.instanceRef].name.in ()));

    if (this->is_local_connection (conn))
      {
        this->disconnect_local_port (plan,
                                     conn,
                                     endpointRef,
                                     receptacle_endpoint);
      }
    else
      {
        this->disconnect_non_local (conn, receptacle_endpoint);
      }
  }

#if !defined (CCM_NOEVENT)
  void
  Connection_Handler::disconnect_publisher (const ::Deployment::DeploymentPlan &plan,
                                            ::CORBA::ULong connectionRef,
                                            ::CORBA::ULong endpointRef)

  {
    CIAO_TRACE ("Connection_Handler::disconnect_publisher");

    const ::Deployment::PlanConnectionDescription &conn =
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &endpoint =
      conn.internalEndpoint[endpointRef];

    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                "Connection_Handler::disconnect_publisher - "
                "Disconnecting connection <%C> on instance <%C>. "
                "Portname: [%C]\n",
                conn.name.in (),
                plan.instance[endpoint.instanceRef].name.in (),
                endpoint.portName. in ()));

    if (conn.internalEndpoint.length () == 0)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::disconnect_event_port - "
                        "Error: Expected internal endpoints for connection <%C>\n",
                        conn.name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               "Expected internal endpoints.");
      }
    ::Components::CCMObject_var obj = this->get_ccm_object (conn.name.in ());

    ::Components::EventConsumerBase_var safe_temp =
      obj->unsubscribe (endpoint.portName.in (),
                        this->get_cookie (conn.name.in ()));
  }
#endif

#if !defined (CCM_NOEVENT)
  void
  Connection_Handler::disconnect_emitter (const ::Deployment::DeploymentPlan &,
                                          ::CORBA::ULong,
                                          ::CORBA::ULong)

  {
    CIAO_TRACE ("Connection_Handler::disconnect_emitter");
  }
#endif

  void
  Connection_Handler::connect_local_port (const ::Deployment::DeploymentPlan & plan,
                                          const ::Deployment::PlanConnectionDescription &conn,
                                          ::CORBA::ULong endpointRef,
                                          const ::Deployment::PlanSubcomponentPortEndpoint &receptacle_endpoint)
  {
    CIAO_TRACE ("Connection_Handler::connect_local_port");


    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Connection_Handler::connect_local_port - "
                    "Connecting connection <%C> on instance <%C>\n",
                    conn.name.in (),
                    plan.instance[receptacle_endpoint.instanceRef].name.in ()));

    CORBA::ULong facet_endpointRef = (endpointRef + 1) % 2;
    if (conn.internalEndpoint.length () != 2 ||
          conn.internalEndpoint[facet_endpointRef].kind != ::Deployment::Facet)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_local_port - "
                        "Error: Wrong number of internal endpoints for local connection: "
                        "expected <2> - found <%d>\n",
                        conn.internalEndpoint.length ()));

        throw ::Deployment::InvalidConnection (conn.name.in (),
                                                "Local connections require exactly 2 internalEndpoints");
      }

    const ::Deployment::PlanSubcomponentPortEndpoint &facet_endpoint =
      conn.internalEndpoint[facet_endpointRef];

    const char *facet_cont =
      DEPLOYMENT_STATE::instance ()->instance_to_container (
        plan.instance[facet_endpoint.instanceRef].name.in ());
    const char *recep_cont =
      DEPLOYMENT_STATE::instance ()->instance_to_container (
        plan.instance[receptacle_endpoint.instanceRef].name.in ());

    if (facet_cont && recep_cont &&
        ACE_OS::strcmp (facet_cont, recep_cont) != 0)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_local_port - "
                        "Ports <%C> and <%C> participate in local connection, "
                        "but are installed in differing containers <%C> and <%C>\n",
                        plan.instance[facet_endpoint.instanceRef].name.in (),
                        plan.instance[receptacle_endpoint.instanceRef].name.in (),
                        facet_cont,
                        recep_cont));
        throw ::Deployment::InvalidConnection (plan.instance[facet_endpoint.instanceRef].name.in (),
                                               "Component instance participates in a local connection with "
                                               "a non-local entity.");
      }

    CIAO::Container_var cont =
      DEPLOYMENT_STATE::instance ()->fetch_container (facet_cont);

    Components::CCMObject_var
      facet = DEPLOYMENT_STATE::instance ()->fetch_component (
          plan.instance[facet_endpoint.instanceRef].name.in ()),
      receptacle = DEPLOYMENT_STATE::instance ()->fetch_component (
          plan.instance[receptacle_endpoint.instanceRef].name.in ());

    ::Components::Cookie_var cookie = cont->connect_local_facet (facet,
                                                                 facet_endpoint.portName.in (),
                                                                 receptacle,
                                                                 receptacle_endpoint.portName.in ());
    CIAO_DEBUG (5, (LM_INFO, CLINFO
                    "Connection_Handler::connect_local_port - "
                    "Connected local port <%C>:<%C> to <%C>:<%C>\n",
                    plan.instance[facet_endpoint.instanceRef].name.in (),
                    facet_endpoint.portName.in (),
                    plan.instance[receptacle_endpoint.instanceRef].name.in (),
                    receptacle_endpoint.portName.in ()));

    CONNECTION_INFO conn_info = CONNECTION_INFO (cookie._retn (),
                                                 ::Components::CCMObject::_duplicate (receptacle.in ()));
    this->insert_cookie (conn.name.in (), conn_info);
  }

  void
  Connection_Handler::disconnect_local_port (const ::Deployment::DeploymentPlan & plan,
                                            const ::Deployment::PlanConnectionDescription &conn,
                                            ::CORBA::ULong endpointRef,
                                            const ::Deployment::PlanSubcomponentPortEndpoint &receptacle_endpoint)
  {
    CIAO_TRACE ("Connection_Handler::disconnect_local_port");


    CORBA::ULong facet_endpointRef = (endpointRef + 1) % 2;
    if (conn.internalEndpoint.length () != 2 ||
        conn.internalEndpoint[facet_endpointRef].kind != ::Deployment::Facet)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::disconnect_local_port - "
                        "Error: Wrong number of internal endpoints for local "
                        "connection: expected <2> - found <%d>\n",
                        conn.internalEndpoint.length ()));

        throw ::Deployment::InvalidConnection (conn.name.in (),
                                                "Local connections require exactly 2 "
                                                "internalEndpoints");

      }
    const ::Deployment::PlanSubcomponentPortEndpoint &facet_endpoint =
      conn.internalEndpoint[facet_endpointRef];

    const char *facet_cont =
      DEPLOYMENT_STATE::instance ()->instance_to_container (
        plan.instance[facet_endpoint.instanceRef].name.in ());
    const char *recep_cont =
      DEPLOYMENT_STATE::instance ()->instance_to_container (
        plan.instance[receptacle_endpoint.instanceRef].name.in ());

    if (facet_cont && recep_cont &&
        ACE_OS::strcmp (facet_cont, recep_cont) != 0)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::disconnect_local_port - "
                        "Ports <%C> and <%C> participate in local connection, "
                        "but are installed in differing containers <%C> and <%C>\n",
                        plan.instance[facet_endpoint.instanceRef].name.in (),
                        plan.instance[receptacle_endpoint.instanceRef].name.in (),
                        facet_cont,
                        recep_cont));
        throw ::Deployment::InvalidConnection (plan.instance[facet_endpoint.instanceRef].name.in (),
                                               "Component instance participates in a local connection with "
                                               "a non-local entity.");
      }

    CIAO::Container_var cont =
      DEPLOYMENT_STATE::instance ()->fetch_container (facet_cont);

    Components::CCMObject_var
      facet = DEPLOYMENT_STATE::instance ()->fetch_component (
        plan.instance[facet_endpoint.instanceRef].name.in ()),
      receptacle = DEPLOYMENT_STATE::instance ()->fetch_component (
        plan.instance[receptacle_endpoint.instanceRef].name.in ());

    cont->disconnect_local_facet (
                               this->get_cookie (conn.name.in ()),
                               facet,
                               facet_endpoint.portName.in (),
                               receptacle,
                               receptacle_endpoint.portName.in ());

    this->remove_cookie (conn.name.in ());

    CIAO_DEBUG (5, (LM_INFO, CLINFO
                    "Connection_Handler::disconnect_local_port - "
                    "Disconnected local port <%C>:<%C> to <%C>:<%C>\n",
                    plan.instance[facet_endpoint.instanceRef].name.in (),
                    facet_endpoint.portName.in (),
                    plan.instance[receptacle_endpoint.instanceRef].name.in (),
                    receptacle_endpoint.portName.in ()));
  }

  bool
  Connection_Handler::is_local_connection (const ::Deployment::PlanConnectionDescription &conn)
  {
    CIAO_TRACE ("Connection_Handler::is_local_connection");

    Deployment::Requirements const& deploy_req = conn.deployRequirement;
    for (CORBA::ULong i = 0; i < deploy_req.length (); i++)
      {
        if (ACE_OS::strcmp (deploy_req[i].name, "edu.dre.vanderbilt.DAnCE.ConnectionType") == 0 &&
            ACE_OS::strcmp (deploy_req[i].resourceType, "Local_Interface") == 0)
          {
            return true;
          }
      }
    return false;
  }

  void
  Connection_Handler::insert_cookie (const char * connection_name,
                                     const CONNECTION_INFO conn_info)
  {
    CIAO_TRACE ("Connection_Handler::insert_cookie");

    std::pair <std::string, CONNECTION_INFO> value_to_insert (connection_name,
                                                              conn_info);
    std::pair<COOKIES::iterator, bool> ret = this->cookies_.insert (value_to_insert);
    if (!ret.second)
      {
        CIAO_ERROR (1, (LM_ERROR,  CLINFO
                        "Connection_Handler::insert_cookie - "
                        "Error inserting new cookie\n"));
        conn_info.first->_remove_ref ();
        conn_info.second->_remove_ref ();
        throw ::Deployment::InvalidConnection (connection_name,
                                               "Unable to insert cookie.");
      }
    else
      {
        CIAO_DEBUG (5, (LM_DEBUG, CLINFO
                        "Connection_Handler::insert_cookie - "
                        "Inserted cookie for [%C].\n",
                        connection_name));
      }
  }

  void
  Connection_Handler::remove_cookie (const char* connection_name)
  {
    CIAO_TRACE ("Connection_Handler::remove_cookie");

    COOKIES::iterator it = this->cookies_.find (connection_name);
    if (it == this->cookies_.end ())
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::remove_cookie - "
                        "Unable to delete cookie for connection <%C>\n",
                        connection_name));
        return;
      }
    CIAO_DEBUG (5, (LM_DEBUG, CLINFO
                    "Connection_Handler::remove_cookie - "
                    "About to remove cookie for [%C].\n",
                    connection_name));
    it->second.second = ::Components::CCMObject::_nil ();
    this->cookies_.erase (it);
  }

  ::Components::Cookie *
  Connection_Handler::get_cookie (const char * connection_name)
  {
    CIAO_TRACE ("Connection_Handler::get_cookie");

    COOKIES::iterator it = this->cookies_.find (connection_name);
    if (it == this->cookies_.end ())
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::get_cookie - "
                        "Cookie for <%C> not found\n",
                        connection_name));
        throw ::Deployment::InvalidConnection (connection_name,
                                               "Unable to find correct cookie");
      }
    else
      {
        CIAO_DEBUG (5, (LM_DEBUG, CLINFO
                        "Connection_Handler::get_cookie - "
                        "Found cookie for [%C]\n",
                        connection_name));
      }
    return it->second.first.in ();
  }

  ::Components::CCMObject_ptr
  Connection_Handler::get_ccm_object (const char * connection_name)
  {
    COOKIES::iterator it = this->cookies_.find (connection_name);
    if (it == this->cookies_.end ())
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::get_ccm_object - "
                        "Cookie for <%C> not found\n",
                        connection_name));
        throw ::Deployment::InvalidConnection (connection_name,
                                               "Unable to find correct cookie");
      }
    ::Components::CCMObject_var ret = it->second.second;
    if (::CORBA::is_nil (ret.in ()))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::get_ccm_object - "
                        "Error: Stored CCM object seems to be nil.\n",
                        connection_name));
        throw ::Deployment::InvalidConnection (connection_name,
                                               "Stored CCM object seems nil");
      }
    return ::Components::CCMObject::_duplicate (ret.in ());
  }

  ::CORBA::ULong
  Connection_Handler::retrieve_endpoint (const ::Deployment::PlanConnectionDescription &conn)
  {
    if (conn.internalEndpoint.length () == 0)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::retrieve_endpoint - "
                        "Connection <%C> lacks an internalEndpoint.\n",
                        conn.name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                       "No internal endpoint for connection\n");
      }
    if (conn.internalEndpoint.length () > 1)
      {
        for (CORBA::ULong i = 0;
             i < conn.internalEndpoint.length ();
             ++i)
          {
            if (!conn.internalEndpoint[i].provider)
              return i;
          }
      }
    return 0;
  }

}
