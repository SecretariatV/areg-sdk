//////////////////////////////////////////////////////////////////////////
// Begin generate generated/src/HelloServiceClientBase.hpp file
//////////////////////////////////////////////////////////////////////////
#pragma once

/************************************************************************
 * (c) copyright    2021
 *
 * Generated at     20.10.2021  21:36:19 GMT+02:00 
 *                  Create by AREG SDK code generator tool from source HelloService.
 *
 * \file            generated/src/HelloServiceClientBase.hpp
 * \ingroup         HelloService Service Interface
 * \brief           This is an automatic generated code of HelloService
 *                  Service Interface Client base class declaration.
 ************************************************************************/

/************************************************************************
 * Include files
 ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "generated/src/NEHelloService.hpp"
#include "areg/component/IEProxyListener.hpp"
#include "areg/component/NotificationEvent.hpp"

#include "generated/src/private/HelloServiceProxy.hpp"

/************************************************************************
 * Dependencies
 ************************************************************************/
class Component;
class DispatcherThread;

//////////////////////////////////////////////////////////////////////////
// HelloServiceClientBase class declaration.
//////////////////////////////////////////////////////////////////////////
/**
 * \brief       The interface of HelloService Service Interface
 *              Client base object. This object should be inherited
 *              and overrides should be implemented.
 *
 *              The hello world application
 **/
class HelloServiceClientBase  : public IEProxyListener
{
//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor. Protected
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief   Initialize HelloService Service Interface client object. 
     *          Specifies used service and owner thread.
     * \param   roleName    The role name assigned to HelloService servicing component object.
     * \param   ownerThread The name of component owner thread to dispatch messages.
     *                      If nullptr, all messages are dispatched in current component thread.
     **/
    HelloServiceClientBase( const char* roleName, const char * ownerThread = nullptr );

    /**
     * \brief   Initialize HelloService Service Interface client object.
     *          Specifies used service and owner thread.
     * \param   roleName    The role name assigned to HelloService servicing component object.
     * \param   ownerThread The instance of component owner thread to dispatch messages.
     **/
    HelloServiceClientBase( const char* roleName, DispatcherThread & ownerThread );

    /**
     * \brief   Initialize HelloService Service Interface client object.
     *          Specifies used service and owner component.
     * \param   roleName    The role name assigned to HelloService servicing component object.
     * \param   owner       The instance of client owner component. The component object should be already initialized.
     * \note    When this constructor is used, it is important that the Component object is already initialized.
     *          and the component thread is set.
     **/
    HelloServiceClientBase( const char* roleName, Component & owner );

    /**
     * \brief   Destructor.
     **/
    virtual ~HelloServiceClientBase( void );

//////////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Clears all notifications, stops receiving notifications from server
     **/
    inline void clearAllNotifications( void );

    /**
     * \brief   Returns true if the specified certain notification is already assigned.
     *          Otherwise returns false.
     * \param   msgId   The ID of message to check.
     **/
    inline bool isNotificationAssigned( NEHelloService::eMessageIDs msgId ) const;

    /**
     * \brief   Returns true if client object has got connection with servicing component
     **/
    inline bool isConnected( void ) const;
    
    /**
     * \brief   Returns the name of used service.
     **/
    inline const String & getServiceName( void ) const;
    
    /**
     * \brief   Returns the version of used service.
     **/
    inline const Version & getServiceVersion( void ) const;

//////////////////////////////////////////////////////////////////////////
// Start Service Interface operations / attributes and overrides declaration
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// HelloService Interface Attributes
//////////////////////////////////////////////////////////////////////////
public:

//////////////////////////////////////////////////////////////////////////
// HelloService Interface Requests / Responses / Broadcasts
//////////////////////////////////////////////////////////////////////////
public:

/************************************************************************
 * Request HelloService
 ************************************************************************/
    /**
     * \brief   Request call.
     *          The request to output greeting.
     * \param   client  The name of client to output greeting.
     * \return  The sequence count number of call
     * \see     responseHelloService
     **/
    inline unsigned int requestHelloService( const String & client );
    /**
     * \brief   Overwrite to handle error of HelloService request call.
     * \param   FailureReason   The failure reason value of request call.
     **/
    virtual void requestHelloServiceFailed( NEService::eResultType FailureReason );
    
/************************************************************************
 * Response HelloService
 ************************************************************************/
    /**
     * \brief   Response callback.
     *          The response indicating success status to output greeting
     *          Overwrite, if need to handle Response call of server object. 
     *          This call will be automatically triggered, on every appropriate request call
     * \param   success Flag, indicates the success of output.
     * \see     requestHelloService
     **/
    virtual void responseHelloService( bool success );
    /**
     * \brief   Call to enable or disable receiving notifications on HelloService response call.
     *          This function is triggered, when client object is interested only on response result
     *          without triggering request call.
     * \param   notify  If true, notification will be enable. If false, notification is disabled
     **/
    inline void notifyOnResponseHelloService( bool notify = true );

//////////////////////////////////////////////////////////////////////////
// End Service Interface operations / attributes and overrides declaration
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////////
protected:
/************************************************************************/
// IEProxyListener Overrides
/************************************************************************/
    /**
     * \brief   Triggered by Proxy, when gets service connected event.
     *          Make client initializations in this function. No request
     *          will be processed until this function is not called for
     *          client object. Also set listeners here if client is interested
     *          to receive update notifications.
     * \param   isConnected     The flag, indicating whether service is connected
     *                          or disconnected. Make cleanups and stop sending
     *                          requests or assigning for notification if
     *                          this flag is false. No event to Stub target will
     *                          be sent, until no service connected event is
     *                          received.
     * \param   proxy           The Service Interface Proxy object, which is
     *                          notifying service connection.
     * \return  Return true if this service connect notification was relevant to client object,
     *          i.e. if passed Proxy address is equal to the Proxy object that client has.
     *          If Proxy objects are not equal, it should return false;
     **/
    virtual bool serviceConnected( bool isConnected, ProxyBase & proxy ) override;

/************************************************************************/
// HelloServiceClientBase Error Handling overrides
/************************************************************************/

    /**
     * \brief   Overwrite this method if need to make error handling on invalid request
     * \param   InvalidReqId    The ID of invalid request
     **/
    virtual void invalidRequest( NEHelloService::eMessageIDs InvalidReqId );
    
//////////////////////////////////////////////////////////////////////////
// Attributes
//////////////////////////////////////////////////////////////////////////

    /**
     * \brief   Call to recreate Proxy for the client. This call will remove and unregister all existing notifications
     *          and the client will not receive pending update or response notifications. 
     *          The client first will receive disconnect message, then again connect.
     *          Reset update notifications manually in connect if need.          
     *          This function call can be used when the client instance should be registered and run in new thread.
     *
     * \return  Returns true if Proxy was created with success.
     **/
    bool recreateProxy( void );

    /**
     * \brief  Returns pointer to client dispatcher thread where the messages are dispatched.
     *         The function can return nullptr if Proxy was not instantiated yet.
     **/
    DispatcherThread * getDispatcherThread( void );
     
    /**
     * \brief   Returns the current sequence number
     **/
    inline unsigned int getCurrentSequenceNr( void ) const;

    /**
     * \brief  Returns instance of proxy object.
     */
    inline const HelloServiceProxy * getProxy( void ) const;
      
    /**
     * \brief Returns target service component role name.
     **/
    inline const String & getServiceRole( void ) const;

//////////////////////////////////////////////////////////////////////////
// Member variables
//////////////////////////////////////////////////////////////////////////
private:
    /**
     * \brief   Connection flag. If true, the client object is connected to server
     **/
    bool                mIsConnected;
    /**
     * \brief   The counter of sequence number
     **/
    unsigned int        mCurrSequenceNr;
    /**
     * \brief   Pointer of Proxy object providing communication
     **/
    HelloServiceProxy *   mProxy;

//////////////////////////////////////////////////////////////////////////
// Hidden methods
//////////////////////////////////////////////////////////////////////////
private:
/************************************************************************/
// IENotificationEventConsumer overrides
/************************************************************************/

    /**
     * \brief   Is processing notification event calls.
     * \param   eventElem   Notification Event object to process
     **/
    virtual void processNotificationEvent( NotificationEvent & eventElem ) override;
    
/************************************************************************/
// HelloServiceClientBase hidden methods
/************************************************************************/

    /**
     * \brief   Enables / Disables notification flags on appropriate message call.
     * \param   msgId   The ID of message to enable / disable notification
     * \param   notify  If true, the notification is enabled. 
     *                  If false, the notification is disabled.
     * \param   always  Flag, indicating whether should notify always or not.
     *                  if 'notify' parameter is true and the notification is already
     *                  assigned, then if parameter 'always' is true, it will trigger
     *                  notification immediately after call. 
     **/
    void notifyOn( NEHelloService::eMessageIDs msgId, bool notify, bool always = false );
    /**
     * \brief   Overwrite this method if need to make error handling on invalid response
     * \param   InvalidRespId   The ID of invalid response
     **/
     void invalidResponse( NEHelloService::eMessageIDs InvalidRespId );

    /**
     * \brief   By default, the function calls appropriate request failure function.
     *          Overwrite this method if need to make error handling on request failed.
     * \param   msgId           The ID of either response of request message, which failed. Normally ID of request.
     * \param   FailureReason   The failure reason value of request call.
     **/
    void requestFailed( NEHelloService::eMessageIDs FailureMsgId, NEService::eResultType FailureReason );

    /**
     * \brief   Returns reference of HelloServiceClientBase object
     **/

    inline HelloServiceClientBase & self( void );

//////////////////////////////////////////////////////////////////////////
// Forbidden calls
//////////////////////////////////////////////////////////////////////////
private:
    HelloServiceClientBase( void ) = delete;
    DECLARE_NOCOPY_NOMOVE( HelloServiceClientBase );
};

//////////////////////////////////////////////////////////////////////////
// HelloServiceClientBase class inline functions implementation
//////////////////////////////////////////////////////////////////////////

inline HelloServiceClientBase & HelloServiceClientBase::self( void )
{
    return (*this);
}

inline unsigned int HelloServiceClientBase::getCurrentSequenceNr( void ) const
{
    return mCurrSequenceNr;
}

inline void HelloServiceClientBase::clearAllNotifications( void )
{
    ASSERT(mProxy != nullptr);
    mProxy->clearAllNotifications(static_cast<IENotificationEventConsumer &>(self()));
}

inline bool HelloServiceClientBase::isConnected( void ) const
{
    ASSERT(mProxy != nullptr);
    return mIsConnected;
}

inline bool HelloServiceClientBase::isNotificationAssigned( NEHelloService::eMessageIDs msgId ) const
{
    ASSERT(mProxy != nullptr);
    return mProxy->hasNotificationListener(static_cast<unsigned int>(msgId));
}

inline const String & HelloServiceClientBase::getServiceName( void ) const
{
    ASSERT(mProxy != nullptr);
    return mProxy->getProxyAddress().getServiceName();
}
    
inline const Version & HelloServiceClientBase::getServiceVersion( void ) const
{
    ASSERT(mProxy != nullptr);
    return mProxy->getProxyAddress().getServiceVersion();
}

/************************************************************************
 * Request calls
 ************************************************************************/

inline unsigned int HelloServiceClientBase::requestHelloService( const String & client )
{
    ASSERT(mProxy != nullptr);
    return mProxy->requestHelloService( static_cast<IENotificationEventConsumer &>(self()), client );
}

/************************************************************************
 * Response notifications
 ************************************************************************/

inline void HelloServiceClientBase::notifyOnResponseHelloService( bool notify /* = true */ )
{
    HelloServiceClientBase::notifyOn(NEHelloService::eMessageIDs::MsgId_responseHelloService, notify, false);
}

inline const HelloServiceProxy * HelloServiceClientBase::getProxy( void ) const
{
    return mProxy;
}

inline const String & HelloServiceClientBase::getServiceRole( void ) const
{
    ASSERT(mProxy != nullptr);
    return mProxy->getProxyAddress().getRoleName();
}

//////////////////////////////////////////////////////////////////////////
// End generate generated/src/HelloServiceClientBase.hpp file
//////////////////////////////////////////////////////////////////////////
