#include <atlbase.h>
#include "mbnapi.h"
#include <tchar.h>
#include <string>
#include <iostream>
#include "stdio.h"
#include "wbemidl.h"
#include "comdef.h"
#include <comutil.h>
#include <WinBase.h>

#include<Windows.h>
#include<WinInet.h>
#include<strsafe.h>

#include "Netcon.h"
#include <stdlib.h>  

#pragma comment(lib, "mbnapi_uuid")
#pragma comment(lib, "comsuppw")
#pragma comment(lib, "comsuppwd")
#pragma comment(lib, "wininet")
#pragma comment(lib, "Kernel32")

std::string toString(LPWSTR data)
{
	std::string convertedData = "<empty>";
	size_t size = wcslen(data);
	if (size > 0) convertedData = "";
	for (int s = 0; s <= size; ++s)
	{
		convertedData += static_cast<char>(data[s]);
	}
	return convertedData;
}


typedef struct{
    HWND       hWindow;      // Window handle
    int        nStatusList;  // List box control to hold callbacks
    HINTERNET  hResource;    // HINTERNET handle created by InternetOpenUrl
    char       szMemo[512];  // String to store status memo
} REQUEST_CONTEXT;


void CALLBACK CallMaster( HINTERNET,
                          DWORD_PTR,
                          DWORD,
                          LPVOID,
                          DWORD );

void __stdcall CallMaster(
    HINTERNET hInternet,
    DWORD_PTR dwContext,
    DWORD dwInternetStatus,
    LPVOID lpvStatusInformation,
    DWORD dwStatusInformationLength
)
{
    UNREFERENCED_PARAMETER(hInternet);
    UNREFERENCED_PARAMETER(lpvStatusInformation);
    UNREFERENCED_PARAMETER(dwStatusInformationLength);

    REQUEST_CONTEXT *cpContext;
    cpContext = (REQUEST_CONTEXT*)dwContext;
    char szStatusText[80];

    switch (dwInternetStatus)
    {
        case INTERNET_STATUS_CLOSING_CONNECTION:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s CLOSING_CONNECTION",
                              cpContext->szMemo);
            break;
        case INTERNET_STATUS_CONNECTED_TO_SERVER:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s CONNECTED_TO_SERVER",
                              cpContext->szMemo );
            break;
        case INTERNET_STATUS_CONNECTING_TO_SERVER:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s CONNECTING_TO_SERVER",
                              cpContext->szMemo );
            break;
        case INTERNET_STATUS_CONNECTION_CLOSED:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s CONNECTION_CLOSED",
                              cpContext->szMemo );
            break;
        case INTERNET_STATUS_HANDLE_CLOSING:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s HANDLE_CLOSING",
                              cpContext->szMemo );
            break;
        case INTERNET_STATUS_HANDLE_CREATED:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s HANDLE_CREATED",
                              cpContext->szMemo);
            break;
        case INTERNET_STATUS_INTERMEDIATE_RESPONSE:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s INTERMEDIATE_RESPONSE",
                              cpContext->szMemo );
            break;
        case INTERNET_STATUS_NAME_RESOLVED:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s NAME_RESOLVED",
                              cpContext->szMemo);
            break;
        case INTERNET_STATUS_RECEIVING_RESPONSE:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s RECEIVING_RESPONSE",
                              cpContext->szMemo);
            break;
        case INTERNET_STATUS_RESPONSE_RECEIVED:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s RESPONSE_RECEIVED",
                              cpContext->szMemo);
            break;
        case INTERNET_STATUS_REDIRECT:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s REDIRECT",
                              cpContext->szMemo );
            break;
        case INTERNET_STATUS_REQUEST_COMPLETE:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s REQUEST_COMPLETE",
                              cpContext->szMemo);
            break;
        case INTERNET_STATUS_REQUEST_SENT:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s REQUEST_SENT",
                              cpContext->szMemo);
            break;
        case INTERNET_STATUS_RESOLVING_NAME:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s RESOLVING_NAME",
                              cpContext->szMemo );
            break;
        case INTERNET_STATUS_SENDING_REQUEST:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s SENDING_REQUEST",
                              cpContext->szMemo );
            break;
        case INTERNET_STATUS_STATE_CHANGE:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s STATE_CHANGE",
                              cpContext->szMemo );
            break;
        default:
            StringCchPrintfA( szStatusText,
                              80,
                              "%s Unknown Status %d Given",
                              cpContext->szMemo,
                              dwInternetStatus);
            break;
    }

    SendDlgItemMessage( cpContext->hWindow,
                      cpContext->nStatusList,
                      LB_ADDSTRING,
                      0, (LPARAM)szStatusText );

}

class clsPinEvents : public IMbnPinEvents
{

	//
	private:
		long m_cRef;

	public:
		//ULONG STDMETHODCALLTYPE AddRef(){

		//	m_cRef++;
		//	return m_cRef;
		//}
		//ULONG STDMETHODCALLTYPE Release(){
		//	ULONG l;
		//	l = m_cRef--;
		//	if ( 0 == m_cRef)
		//	{
		//		delete this;
		//	}
		//	//return m_cRef;
		//	return l-1;
		//}
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv){
				if (IsEqualGUID(riid, IID_IMbnPinEvents)) {
				*ppv = static_cast<IMbnPinEvents*>(this);
				AddRef();
				return S_OK;
				}


				return E_NOINTERFACE;

	}
	   HRESULT STDMETHODCALLTYPE OnInterfaceCapabilityAvailable( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface)
	   {
		   return S_OK;
	   }     
	   HRESULT STDMETHODCALLTYPE OnSubscriberInformationChange( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface)
				   {
		   return S_OK;
	   }       
		HRESULT STDMETHODCALLTYPE OnReadyStateChange( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface)
				   {
		   return S_OK;
	   }       
		HRESULT STDMETHODCALLTYPE OnEmergencyModeChange( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface)
				   {
		   return S_OK;
	   }       
		HRESULT STDMETHODCALLTYPE OnHomeProviderAvailable( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface)
				   {
		   return S_OK;
	   }       
		HRESULT STDMETHODCALLTYPE OnPreferredProvidersChange( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface)
				   {
		   return S_OK;
	   }     
		HRESULT STDMETHODCALLTYPE OnSetPreferredProvidersComplete( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface,
			/* [in] */ ULONG requestID,
			/* [in] */ HRESULT status)
				   {
		   return S_OK;
	   }      
		HRESULT STDMETHODCALLTYPE OnScanNetworkComplete( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface,
			/* [in] */ ULONG requestID,
			/* [in] */ HRESULT status)
				   {
		return S_OK;
		}


	//
	//private:
	//	long m_cRef;

	public:
		HANDLE EnterPinComplete;
		clsPinEvents() {
		EnterPinComplete = CreateEvent(NULL, true, false, NULL);}

		ULONG STDMETHODCALLTYPE AddRef(){

			m_cRef++;
			return m_cRef;
		}
		ULONG STDMETHODCALLTYPE Release(){
			ULONG l;
			l = m_cRef--;
			if ( 0 == m_cRef)
			{
				delete this;
			}
			//return m_cRef;
			return l-1;
		}
		//HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv){
		//if (IsEqualGUID(riid, IID_IMbnPinEvents)) 
		//{
		//	*ppv = static_cast<IMbnPinEvents*>(this);
		//	AddRef();
		//	return S_OK;
		//}
		//return E_NOINTERFACE;

	//}
	

		/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnEnableComplete( 
            /* [in] */ __RPC__in_opt IMbnPin *pin,
            /* [ref][in] */ __RPC__in MBN_PIN_INFO *pinInfo,
            /* [in] */ ULONG requestID,
            /* [in] */ HRESULT status)
		{
			return S_OK;
		}
        
         /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnDisableComplete( 
            /* [in] */ __RPC__in_opt IMbnPin *pin,
            /* [ref][in] */ __RPC__in MBN_PIN_INFO *pinInfo,
            /* [in] */ ULONG requestID,
            /* [in] */ HRESULT status)
		 {
			 return S_OK;
		 }
        
         /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnEnterComplete( 
            /* [in] */ __RPC__in_opt IMbnPin *Pin,
            /* [ref][in] */ __RPC__in MBN_PIN_INFO *pinInfo,
            /* [in] */ ULONG requestID,
            /* [in] */ HRESULT status)
		 {
			 return S_OK;
		 }
        
         /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnChangeComplete( 
            /* [in] */ __RPC__in_opt IMbnPin *Pin,
            /* [ref][in] */ __RPC__in MBN_PIN_INFO *pinInfo,
            /* [in] */ ULONG requestID,
            /* [in] */ HRESULT status)
		 {
			 return S_OK;
		 }
        
         /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnUnblockComplete( 
            /* [in] */ __RPC__in_opt IMbnPin *Pin,
            /* [ref][in] */ __RPC__in MBN_PIN_INFO *pinInfo,
            /* [in] */ ULONG requestID,
            /* [in] */ HRESULT status)
		 {
			 return S_OK;
		 }
	~clsPinEvents() { }

};
class clsInterfaceEvents : public IMbnInterfaceEvents
{
	private:
		long m_cRef;

	public:

		HANDLE ProfileAdded;
		HANDLE evScanComplete;
		clsInterfaceEvents() {
		ProfileAdded = CreateEvent(NULL, true, false, NULL);
		evScanComplete = CreateEvent(NULL, true, false, NULL);}

		ULONG STDMETHODCALLTYPE AddRef(){

			m_cRef++;
			return m_cRef;
		}
		ULONG STDMETHODCALLTYPE Release(){
			ULONG l;
			l = m_cRef--;
			if ( 0 == m_cRef)
			{
				delete this;
			}
			//return m_cRef;
			return l-1;
		}
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv){
				if (IsEqualGUID(riid, IID_IMbnInterfaceEvents)) {
				*ppv = static_cast<IMbnInterfaceEvents*>(this);
				AddRef();
				return S_OK;
				}


				return E_NOINTERFACE;

	}
	   HRESULT STDMETHODCALLTYPE OnInterfaceCapabilityAvailable( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface)
	   {
		   return S_OK;
	   }     
	   HRESULT STDMETHODCALLTYPE OnSubscriberInformationChange( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface)
				   {
		   return S_OK;
	   }       
		HRESULT STDMETHODCALLTYPE OnReadyStateChange( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface)
				   {
		   return S_OK;
	   }       
		HRESULT STDMETHODCALLTYPE OnEmergencyModeChange( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface)
				   {
		   return S_OK;
	   }       
		HRESULT STDMETHODCALLTYPE OnHomeProviderAvailable( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface)
				   {
		   return S_OK;
	   }       
		HRESULT STDMETHODCALLTYPE OnPreferredProvidersChange( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface)
				   {
		   return S_OK;
	   }     
		HRESULT STDMETHODCALLTYPE OnSetPreferredProvidersComplete( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface,
			/* [in] */ ULONG requestID,
			/* [in] */ HRESULT status)
				   {
		   return S_OK;
	   }      
		HRESULT STDMETHODCALLTYPE OnScanNetworkComplete( 
			/* [in] */ __RPC__in_opt IMbnInterface *newInterface,
			/* [in] */ ULONG requestID,
			/* [in] */ HRESULT status)
				   {
			SetEvent(evScanComplete);
		   return S_OK;
	   }

	
		~clsInterfaceEvents() { };
};
class clsInterfaceManagerEvents : public IMbnInterfaceManagerEvents
{
	private:
		long m_cRef;

public:
	clsInterfaceManagerEvents(){}

	ULONG STDMETHODCALLTYPE AddRef(){

		m_cRef++;
		return m_cRef;
	}
	ULONG STDMETHODCALLTYPE Release(){
		ULONG l;
		l = m_cRef--;
		if ( 0 == m_cRef)
		{
			delete this;
		}
		//return m_cRef;
		return l-1;
	}
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv){
			if (IsEqualGUID(riid, IID_IMbnInterfaceManagerEvents)) {
			*ppv = static_cast<IMbnInterfaceManagerEvents*>(this);
			AddRef();
			return S_OK;
			}
			return E_NOINTERFACE;

	}

	/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnInterfaceArrival( 
            /* [in] */ __RPC__in_opt IMbnInterface *newInterface)
	{
		return S_OK;
	}
        
    /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnInterfaceRemoval( 
            /* [in] */ __RPC__in_opt IMbnInterface *oldInterface)
	{
		return S_OK;
	}

	~clsInterfaceManagerEvents() {};
};
class clsConnectionProfileEvents : public IMbnConnectionProfileEvents
{
	private:
		long m_cRef;

	public:
	HANDLE ProfileUpdated;
	clsConnectionProfileEvents() {
	ProfileUpdated = CreateEvent(NULL, true, false, NULL);}

	ULONG STDMETHODCALLTYPE AddRef(){

		m_cRef++;
		return m_cRef;
	}
	ULONG STDMETHODCALLTYPE Release(){
		ULONG l;
		l = m_cRef--;
		if ( 0 == m_cRef)
		{
			delete this;
		}
		//return m_cRef;
		return l-1;
	}
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv){
			if (IsEqualGUID(riid, IID_IMbnConnectionProfileEvents)) {
			*ppv = static_cast<IMbnConnectionProfileEvents*>(this);
			AddRef();
			return S_OK;
			}
			return E_NOINTERFACE;

	}


	HRESULT STDMETHODCALLTYPE OnProfileUpdate(IMbnConnectionProfile *newProfile)
	{
		SetEvent(ProfileUpdated);
		return S_OK;
	}
	
	~clsConnectionProfileEvents() { }

};
class clsConnectionProfileManagerEvents : public IMbnConnectionProfileManagerEvents
{
private:
	long m_cRef;
public:
	HANDLE ProfileAdded;
	clsConnectionProfileManagerEvents() {
	ProfileAdded = CreateEvent(NULL, true, false, NULL);}

	ULONG STDMETHODCALLTYPE AddRef(){

		m_cRef++;
		return m_cRef;
	}
	ULONG STDMETHODCALLTYPE Release(){
		ULONG l;
		l = m_cRef--;
		if ( 0 == m_cRef)
		{
			delete this;
		}
		//return m_cRef;
		return l-1;
	}
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv){
			if (IsEqualGUID(riid, IID_IMbnConnectionProfileManagerEvents)) {
			*ppv = static_cast<IMbnConnectionProfileManagerEvents*>(this);
			AddRef();
			return S_OK;
			}


			return E_NOINTERFACE;

	}

	HRESULT STDMETHODCALLTYPE OnConnectionProfileArrival(IMbnConnectionProfile *newConnectionProfile)
	{
		SetEvent(ProfileAdded);
		BSTR Profile;
		newConnectionProfile->GetProfileXmlData(&Profile);
		SetEvent(ProfileAdded);
		return S_OK;
	}
	HRESULT STDMETHODCALLTYPE OnConnectionProfileRemoval(IMbnConnectionProfile *oldConnectionProfile)
	{
		return S_OK;
	}
	
	~clsConnectionProfileManagerEvents() { }

};
class clsRadioEvents : public IMbnRadioEvents 
{
public:
		HANDLE DeviceEnabled;
		clsRadioEvents() {
		DeviceEnabled = CreateEvent(NULL, true, false, NULL);}

		ULONG STDMETHODCALLTYPE AddRef(){

			m_cRef++;
			return m_cRef;
		}
		ULONG STDMETHODCALLTYPE Release(){
			ULONG l;
			l = m_cRef--;
			if ( 0 == m_cRef)
			{
				delete this;
			}
			//return m_cRef;
			return l-1;
		}
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv){
			 if (IsEqualGUID(riid, IID_IMbnRadioEvents)) {
				*ppv = static_cast<IMbnRadioEvents*>(this);
				AddRef();
				return S_OK;
			 }


			 return E_NOINTERFACE;

		}
		HRESULT STDMETHODCALLTYPE OnRadioStateChange(IMbnRadio  *newInterface)
		{
			//HRESULT hr;
			return S_OK;
		}
		HRESULT STDMETHODCALLTYPE OnSetSoftwareRadioStateComplete(IMbnRadio  *newInterface, ULONG requestID, HRESULT status)
		{
			SetEvent(DeviceEnabled);
			//HRESULT hr;
			return S_OK;
		}


		~clsRadioEvents() { }

	private:
		long m_cRef;
};
class clsRegistrationEvents : public IMbnRegistrationEvents
{
public:
		clsRegistrationEvents() {}

		ULONG STDMETHODCALLTYPE AddRef(){

			m_cRef++;
			return m_cRef;
		}
		ULONG STDMETHODCALLTYPE Release(){
			ULONG l;
			l = m_cRef--;
			if ( 0 == m_cRef)
			{
				delete this;
			}
			//return m_cRef;
			return l-1;
		}
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv){
			 if (IsEqualGUID(riid, IID_IMbnRegistrationEvents)) {
				*ppv = static_cast<IMbnRegistrationEvents*>(this);
				AddRef();
				return S_OK;
			 }



			 return E_NOINTERFACE;

		}

		HRESULT STDMETHODCALLTYPE OnPacketServiceStateChange(IMbnRegistration *newInterface)
		{
			ULONG dataclasses;
			ULONG CurrentDataclass;
			newInterface->GetAvailableDataClasses(&dataclasses);
			newInterface->GetCurrentDataClass(&CurrentDataclass);
			//HRESULT hr;
			return S_OK;
		}
		HRESULT STDMETHODCALLTYPE OnRegisterModeAvailable(IMbnRegistration *newInterface)
		{
			//HRESULT hr;
			return S_OK;

		}
		HRESULT STDMETHODCALLTYPE OnRegisterStateChange(IMbnRegistration *newInterface)
		{
			//HRESULT hr;
			return S_OK;
		}
		HRESULT STDMETHODCALLTYPE OnSetRegisterModeComplete(IMbnRegistration *newInterface, ULONG requestID, HRESULT status )
		{
			//HRESULT hr;
			ULONG dataclasses;
			ULONG CurrentDataclass;
			newInterface->GetAvailableDataClasses(&dataclasses);
			newInterface->GetCurrentDataClass(&CurrentDataclass);
			//HRESULT hr;
			return S_OK;
		}

		~clsRegistrationEvents() { }

	private:
		long m_cRef;
};
class clsSignalEvents : public IMbnSignalEvents
{
	public:
		clsSignalEvents() {}

		ULONG STDMETHODCALLTYPE AddRef(){

			m_cRef++;
			return m_cRef;
		}
		ULONG STDMETHODCALLTYPE Release(){
			ULONG l;
			l = m_cRef--;
			if ( 0 == m_cRef)
			{
				delete this;
			}
			//return m_cRef;
			return l-1;
		}
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv){
			//IMbnConnectionEvents *events = (IMbnConnectionEvents*)ppv;
			// TODO: List other implemented interfaces in a similar manner.
			 if (IsEqualGUID(riid, IID_IUnknown) || IsEqualGUID(riid, IID_IMbnSignalEvents)) {
				*ppv = static_cast<IMbnSignalEvents*>(this);
				AddRef();
				return S_OK;
			 }
			 return E_NOINTERFACE;

		}
		////
		HRESULT STDMETHODCALLTYPE OnSignalStateChange(IMbnSignal *newInterface)
		{
			ULONG uSigStrength;
			HRESULT hr;
			hr = newInterface->GetSignalStrength(&uSigStrength);
			return S_OK;
		}

		~clsSignalEvents() { }

	private:
		long m_cRef;
};
class clsConnectionManagerEvents : public IMbnConnectionManagerEvents
{
	public:
		HANDLE ConnectionAdded;
		clsConnectionManagerEvents() { ConnectionAdded  = CreateEvent(NULL, true, false, NULL);}
		

		ULONG STDMETHODCALLTYPE AddRef(){
			m_cRef++;
			return m_cRef;
		}
		ULONG STDMETHODCALLTYPE Release(){
			ULONG l;
			l = m_cRef--;
			if ( 0 == m_cRef)
			{
				delete this;
			}
			//return m_cRef;
			return l-1;
		}
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv){
			//IMbnConnectionEvents *events = (IMbnConnectionEvents*)ppv;
			// TODO: List other implemented interfaces in a similar manner.
			 if (IsEqualGUID(riid, IID_IUnknown) || IsEqualGUID(riid, IID_IMbnConnectionManagerEvents)) {
				*ppv = static_cast<IMbnConnectionManagerEvents*>(this);
				AddRef();
				return S_OK;
			 }
			 return E_NOINTERFACE;
		}
		////
		HRESULT STDMETHODCALLTYPE OnConnectionArrival(IMbnConnection *newConnection)
		{
			SetEvent(ConnectionAdded);
			return S_OK;
		}
		HRESULT STDMETHODCALLTYPE OnConnectionRemoval(IMbnConnection *newConnection)
		{
			return S_OK;
		}

		~clsConnectionManagerEvents() { }

	private:
		long m_cRef;
};
class clsConnectionEvents : public IMbnConnectionEvents
{
	public:
		clsConnectionEvents() {}

		ULONG STDMETHODCALLTYPE AddRef()
		{
			m_cRef++;
			return m_cRef;
		}
		ULONG STDMETHODCALLTYPE Release()
		{
			ULONG l;
			l = m_cRef--;
			if ( 0 == m_cRef)
			{
				delete this;
			}
			//return m_cRef;
			return l-1;
		}
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv){
			//IMbnConnectionEvents *events = (IMbnConnectionEvents*)ppv;
			// TODO: List other implemented interfaces in a similar manner.
			//IUnknown *punk = nullptr;

			//if (riid == IID_IUnknown)
			//	punk = static_cast<IUnknown*>(this);
			//if (riid == IID_IMbnConnectionEvents)
			//	punk = static_cast<IMbnConnectionEvents*>(this);
			//if (riid == IID_IMbnConnection)
			//	punk = static_cast<IUnknown*>(this);

			//*ppv = punk;
			//if (!punk)
			//	return E_NOINTERFACE;

			//punk->AddRef();
			//return S_OK;
			 if (IsEqualGUID(riid, IID_IUnknown) || IsEqualGUID(riid, IID_IMbnConnectionEvents)) {
				*ppv = static_cast<IMbnConnectionEvents*>(this);
				AddRef();
				return S_OK;
			 }
			 return E_NOINTERFACE;
		}
		////
		HRESULT STDMETHODCALLTYPE OnConnectComplete(IMbnConnection *newConnection, ULONG requestID, HRESULT status)
		{
			MBN_ACTIVATION_STATE state;
			BSTR ProfileName = L"EBITS_MOBILE_BROADBAND_CONNECTION_PROFILE";
			HRESULT hr = E_FAIL;
			hr = newConnection->GetConnectionState(&state, &ProfileName);

			return S_OK;
		}
		HRESULT STDMETHODCALLTYPE OnDisconnectComplete(IMbnConnection *newConnection, ULONG requestID, HRESULT status)
		{
			MBN_ACTIVATION_STATE state;
			BSTR ProfileName;
			HRESULT hr = E_FAIL;
			
			hr = newConnection->GetConnectionState(&state, &ProfileName);
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnConnectStateChange(IMbnConnection *newConnection)
		{
			return S_OK;
		}
		HRESULT STDMETHODCALLTYPE OnVoiceCallStateChange(IMbnConnection *newConnection)
		{
			return S_OK;
		}

		~clsConnectionEvents() { }

	private:
		long m_cRef;
};


IMbnInterfaceManager		 *g_InterfaceMgr		 = NULL;
IMbnConnectionProfileManager *g_ConnectionProfileMgr = NULL;
IMbnConnectionManager		 *g_ConnectionMgr		 = NULL;


LPCWSTR intInterfaceID;
LPCWSTR intConnectionID;

clsInterfaceManagerEvents * InterfaceMgrEv				  = new clsInterfaceManagerEvents();
clsInterfaceEvents *InterfaceEv						      = new clsInterfaceEvents();
clsConnectionManagerEvents *ConnectionMgrEv				  = new clsConnectionManagerEvents();
clsRadioEvents *RadioEv									  = new clsRadioEvents();
clsConnectionProfileManagerEvents *ConnectionProfileMgrEv = new clsConnectionProfileManagerEvents();
clsConnectionProfileEvents *ConnectionProfileEv			  = new clsConnectionProfileEvents();
clsPinEvents *PinEv										  = new clsPinEvents();
clsConnectionEvents *ConnectionEv						  = new clsConnectionEvents();
clsSignalEvents *SignalEv							      = new clsSignalEvents();
clsRegistrationEvents *RegistrationEv				      = new clsRegistrationEvents();

void SetEvents()
{
	HRESULT hr = E_FAIL;
	/////////test/////////
	IConnectionPointContainer *InterfaceMgrPointContainer = NULL;
	IConnectionPointContainer *ConnectionProfileMgrPointContainer = NULL;
	IConnectionPointContainer *ConnectionMgrPointContainer = NULL;
	IConnectionPoint *ConnectionPoint = NULL;

	hr = g_InterfaceMgr->QueryInterface(IID_IConnectionPointContainer, (void **)&InterfaceMgrPointContainer);
	hr = g_ConnectionProfileMgr->QueryInterface(IID_IConnectionPointContainer, (void **)&ConnectionProfileMgrPointContainer);
	hr = g_ConnectionMgr->QueryInterface(IID_IConnectionPointContainer, (void **)&ConnectionMgrPointContainer);

	DWORD pdwCookie;

	// Register for Pin Events
	hr = InterfaceMgrPointContainer->FindConnectionPoint(IID_IMbnPinEvents, &ConnectionPoint);
	hr = ConnectionPoint->Advise(PinEv, &pdwCookie);

	// Register for Interface Manager Events //DO NOT NEED TO DO THIS ONE
	hr = InterfaceMgrPointContainer->FindConnectionPoint(IID_IMbnInterfaceManagerEvents, &ConnectionPoint);
	hr = ConnectionPoint->Advise(InterfaceMgrEv, &pdwCookie);

	// Register for Interface Events //DO NOT NEED TO DO THIS ONE
	hr = InterfaceMgrPointContainer->FindConnectionPoint(IID_IMbnInterfaceEvents , &ConnectionPoint);
	hr = ConnectionPoint->Advise(InterfaceEv, &pdwCookie);

	// Register for Registeration Events 
	hr = InterfaceMgrPointContainer->FindConnectionPoint(IID_IMbnRegistrationEvents, &ConnectionPoint);
	hr = ConnectionPoint->Advise(RegistrationEv, &pdwCookie);

	// Register for Connection Profile Events
	hr = ConnectionProfileMgrPointContainer->FindConnectionPoint(IID_IMbnConnectionProfileEvents  , &ConnectionPoint);
	hr = ConnectionPoint->Advise(ConnectionProfileEv, &pdwCookie);

	// Register for Connection Profile Manager Events
	hr = ConnectionProfileMgrPointContainer->FindConnectionPoint(IID_IMbnConnectionProfileManagerEvents, &ConnectionPoint);
	hr = ConnectionPoint->Advise(ConnectionProfileMgrEv, &pdwCookie);

	// Register for Radio Events
	hr = InterfaceMgrPointContainer->FindConnectionPoint(IID_IMbnRadioEvents, &ConnectionPoint);
	hr = ConnectionPoint->Advise(RadioEv, &pdwCookie);

	// Register for Singal Events //DO NOT NEED TO DO THIS ONE
	hr = InterfaceMgrPointContainer->FindConnectionPoint(IID_IMbnSignalEvents, &ConnectionPoint);;
	hr = ConnectionPoint->Advise(SignalEv, &pdwCookie);

	// Register for Connection Manager Events
	hr = ConnectionMgrPointContainer->FindConnectionPoint(IID_IMbnConnectionManagerEvents, &ConnectionPoint);
	hr = ConnectionPoint->Advise(ConnectionMgrEv, &pdwCookie);

	// Register for Connection Events
	hr = ConnectionMgrPointContainer->FindConnectionPoint(IID_IMbnConnectionEvents, &ConnectionPoint);
	hr = ConnectionPoint->Advise(ConnectionEv, &pdwCookie);

	InterfaceMgrPointContainer->Release();
	ConnectionProfileMgrPointContainer->Release();
	ConnectionMgrPointContainer->Release();
	ConnectionPoint->Release();

	////

	/////////test/////////


	///////////////////
	//IConnectionPointContainer *connPointCon7 = NULL;
	//hr = g_InterfaceMgr->QueryInterface(IID_IConnectionPointContainer, (void **)&connPointCon7);
	//
	//IConnectionPoint *conPoint8 = NULL;
	//hr = connPointCon7->FindConnectionPoint(IID_IMbnPinEvents, &conPoint8);

	//DWORD pdwCookie8;
	//IUnknown *pUnknown5;
	//hr = PinEv->QueryInterface(IID_IUnknown, (void **)&pUnknown5);

	//hr = conPoint8->Advise(PinEv, &pdwCookie8);

	//if(FAILED(hr))
	//{
	//	int failed = 0;
	//}
	//////////////////



	////////////////////

	//IConnectionPointContainer *connPointCon6 = NULL;
	//hr = g_ConnectionProfileMgr->QueryInterface(IID_IConnectionPointContainer, (void **)&connPointCon6);
	//IConnectionPoint *conPoint7 = NULL;
	//hr = connPointCon6->FindConnectionPoint(IID_IMbnConnectionProfileEvents  , &conPoint7);

	//DWORD pdwCookie7;
	//IUnknown *pUnknown4;
	//hr = ConnectionProfileEv->QueryInterface(IID_IUnknown, (void **)&pUnknown4);

	//hr = conPoint7->Advise(ConnectionProfileEv, &pdwCookie7);

	//if(FAILED(hr))
	//{
	//	int failed = 0;
	//}

	////////////////////
	////////////////////
	

	//IConnectionPointContainer *connPointCon5 = NULL;
	//hr = g_InterfaceMgr->QueryInterface(IID_IConnectionPointContainer, (void **)&connPointCon5);
	//IConnectionPoint *conPoint6 = NULL;
	//hr = connPointCon5->FindConnectionPoint(IID_IMbnInterfaceEvents , &conPoint6);

	//DWORD pdwCookie6;

	//IUnknown *pUnknown3;
	//hr = InterfaceEv->QueryInterface(IID_IUnknown, (void **)&pUnknown3);

	//hr = conPoint6->Advise(InterfaceEv, &pdwCookie6);

	//if(FAILED(hr))
	//{
	//	int failed = 0;
	//}


	//////////////////



	//////////////////
	//IConnectionPointContainer *connPointCon4 = NULL;
	//hr = g_ConnectionProfileMgr->QueryInterface(IID_IConnectionPointContainer, (void **)&connPointCon4);
	//IConnectionPoint *conPoint5 = NULL;
	//hr = connPointCon4->FindConnectionPoint(IID_IMbnConnectionProfileManagerEvents, &conPoint5);

	//DWORD pdwCookie5;

	//IUnknown *pUnknown2;
	//hr = ConnectionProfileMgrEv->QueryInterface(IID_IUnknown, (void **)&pUnknown2);

	////hr = conPoint->Advise((IUnknown*)mySink, pdwCookie);
	//hr = conPoint5->Advise(ConnectionProfileMgrEv, &pdwCookie5);

	//if(FAILED(hr))
	//{
	//	int failed = 0;
	//}


	/////////////////
	//////////
	//IConnectionPointContainer *connPointCon3 = NULL;
	//hr = g_InterfaceMgr->QueryInterface(IID_IConnectionPointContainer, (void **)&connPointCon3);
	//IConnectionPoint *conPoint3 = NULL;
	//hr = connPointCon3->FindConnectionPoint(IID_IMbnRegistrationEvents, &conPoint3);

	//DWORD pdwCookie2;
	//IUnknown *pUnknown1;
	//hr = RegistrationEv->QueryInterface(IID_IUnknown, (void **)&pUnknown1);

	////hr = conPoint->Advise((IUnknown*)mySink, pdwCookie);
	//hr = conPoint3->Advise(RegistrationEv, &pdwCookie2);

	//if(FAILED(hr))
	//{
	//	int failed = 0;
	//}
	//test state change
	//IConnectionPointContainer *connPointCon8 = NULL;
	//hr = g_InterfaceMgr->QueryInterface(IID_IConnectionPointContainer, (void **)&connPointCon8);
	//IConnectionPoint *conPoint4 = NULL;
	//hr = connPointCon3->FindConnectionPoint(IID_IMbnRadioEvents, &conPoint4);

	//DWORD pdwCookie3;

	//IUnknown *pUnknown8;
	//hr = RadioEv->QueryInterface(IID_IUnknown, (void **)&pUnknown8);

	////hr = conPoint->Advise((IUnknown*)mySink, pdwCookie);
	//hr = conPoint4->Advise(RadioEv, &pdwCookie3);

	//if(FAILED(hr))
	//{
	//	int failed = 0;
	//}
	//

	/////////


	////////////
	

	//IConnectionPointContainer *connPointCon2 = NULL;
	//hr = g_InterfaceMgr->QueryInterface(IID_IConnectionPointContainer, (void **)&connPointCon2);
	//IConnectionPoint *conPoint2 = NULL;
	//hr = connPointCon2->FindConnectionPoint(IID_IMbnSignalEvents, &conPoint2);

	//DWORD pdwCookie1;

	//IUnknown *pUnknown;
	//hr = SignalEv->QueryInterface(IID_IUnknown, (void **)&pUnknown);

	////hr = conPoint->Advise((IUnknown*)mySink, pdwCookie);
	//hr = conPoint2->Advise(SignalEv, &pdwCookie1);

	//if(FAILED(hr))
	//{
	//	int failed = 0;
	//}

	///////////


	//////////

	//IConnectionPointContainer *connPointCon1 = NULL;
	//hr = g_ConnectionMgr->QueryInterface(IID_IConnectionPointContainer, (void **)&connPointCon1);
	//IConnectionPoint *conPoint1 = NULL;
	//hr = connPointCon1->FindConnectionPoint(IID_IMbnConnectionManagerEvents, &conPoint1);

	//DWORD pdwCookie0;

	//IUnknown *pThinkUnk0;
	//hr = ConnectionMgrEv->QueryInterface(IID_IUnknown,(void **) &pThinkUnk0);
	//hr = conPoint1->Advise(ConnectionMgrEv, &pdwCookie0);

	//if(FAILED(hr))
	//{
	//	int failed = 0;
	//}
	//////////



	/////////////////////
	//IConnectionPointContainer *connPointCon = NULL;
	//hr = g_ConnectionMgr->QueryInterface(IID_IConnectionPointContainer, (void **)&connPointCon);

	//IConnectionPoint *conPoint = NULL;
	//hr = connPointCon->FindConnectionPoint(IID_IMbnConnectionEvents, &conPoint);

	//DWORD pdwCookie00 = 001;

	////CSink *mySink = new CSink();
	//IUnknown *pThinkUnk;
	//hr = ConnectionEv->QueryInterface(IID_IUnknown,(void **) &pThinkUnk);

	////hr = conPoint->Advise((IUnknown*)mySink, pdwCookie);
	//hr = conPoint->Advise(ConnectionEv, &pdwCookie00);

	//if(FAILED(hr))
	//{
	//	int failed = 0;
	//}
	////////////////
}


IMbnInterface *GetAdaptorInterface()
{
	IMbnInterface  *AdaptorInterface = NULL;
	g_InterfaceMgr->GetInterface((LPCWSTR)intInterfaceID, &AdaptorInterface);

	return AdaptorInterface!= NULL ? AdaptorInterface : NULL;
}
IMbnConnection *GetConnectionInterface()
{
	HRESULT hr = E_FAIL;
	int returnVal = -1;

	IMbnConnection  *ConnectionInterface = NULL;
	hr = g_ConnectionMgr->GetConnection((LPCWSTR)intConnectionID, &ConnectionInterface);

	//if we have not manual connected we might not have a connection ID yet so we need to search all connections
	if(FAILED(hr))
	{
		SAFEARRAY *Connections = NULL;

		g_ConnectionMgr->GetConnections(&Connections);
	
		LONG lLower;
		LONG lUpper;
		if (FAILED(hr))
		   returnVal = hr;

		hr = SafeArrayGetLBound(Connections, 1, &lLower);
		if (FAILED(hr))
			returnVal = hr; 

		hr = SafeArrayGetUBound(Connections, 1, &lUpper);
		if (FAILED(hr))
			returnVal = hr; 

		for (LONG l = lLower; l <= lUpper; l++)
		{
			//LPCWSTR ConnectionID;
			hr = SafeArrayGetElement(Connections, &l, (void*)(&ConnectionInterface));
			if (FAILED(hr))
				returnVal = hr; 

			//ConnectionInterface->get_ConnectionID((BSTR*)ConnectionID);
		}
	}

	return ConnectionInterface!= NULL ? ConnectionInterface : NULL;
}
double GetSignalStrength()
{
			//mbnData |= MBN_DATA_CLASS_HSDPA;
		//mdnData &= ~MBN_DATA_CLASS_HSDPA;

		//if( (mbnData & MBN_DATA_CLASS_HSUPA) != 0)
		//{
		//}
	HRESULT hr = E_FAIL;
	int returnVal = -1;
	const double MAX_GSM_SIGNAL_STRENGTH = 31.0;
	IMbnSignal *sigInf = NULL;
	IMbnInterface  *AdaptorInterface = GetAdaptorInterface();

	//Get interface from interface manager
	//g_InterfaceMgr->GetInterface(intInterfaceID, &AdaptorInterface);
	//Get interface for signal
	hr = AdaptorInterface->QueryInterface(IID_IMbnSignal, (void**)&sigInf);

	ULONG sigStrength = 0;
	if(FAILED(hr))
		returnVal = hr;

	//Get singnal strength
	hr = sigInf->GetSignalStrength(&sigStrength);

	if(SUCCEEDED(hr))
	{
		if (sigStrength == MBN_RSSI_UNKNOWN)
			std::cout << "UNKNOWN" << std::endl;
		else
		{
			std::cout << " Signal Strength: " + sigStrength 
			<< "(" 
			<< 100.0*(double)sigStrength/MAX_GSM_SIGNAL_STRENGTH 
			<< "%)" << std::endl;
		}
	}
	else
		returnVal = hr;

	// clean up
	AdaptorInterface->Release();
	sigInf->Release();

	return 100.0*(double)sigStrength/MAX_GSM_SIGNAL_STRENGTH;
}
std::string GetProviderName()
{
	HRESULT hr;
	int returnVal = -1;
	IMbnRegistration *regInf = NULL;
	BSTR proName;

	//Get interface
	IMbnInterface *AdaptorInterface = GetAdaptorInterface();

	//Get registration interface
	hr = AdaptorInterface->QueryInterface(IID_IMbnRegistration,(void**)&regInf);
	if (FAILED(hr))
		returnVal = hr; 

	hr = regInf->GetProviderName(&proName);
	if (FAILED(hr))
		returnVal = hr;

	_tprintf(_T("Provider Name: %s\n"), proName);

	// Clean up
	regInf->Release();
	AdaptorInterface->Release();

	return "";

}
void SetGPRS_2G()
{
	HRESULT hr;
	int returnVal = -1;
	IMbnRegistration *regInf = NULL;
	MBN_REGISTER_MODE regMode = MBN_REGISTER_MODE_MANUAL;
	ULONG regID = 0;
	BSTR proID = 0;

	//Get interface
	IMbnInterface *AdaptorInterface = GetAdaptorInterface();
	
	//Get registration interface
	hr = AdaptorInterface->QueryInterface(IID_IMbnRegistration,(void**)&regInf);
	if (FAILED(hr))
		returnVal = hr; 

	//Get Provider ID
	hr = regInf->GetProviderID(&proID);

	// Set Allowed GPRS speeds
	hr = regInf->SetRegisterMode(regMode, proID,MBN_DATA_CLASS_GPRS | MBN_DATA_CLASS_EDGE, &regID);

	//clean up
	AdaptorInterface->Release();
	regInf->Release();
}
void SetGPRS_3G_Full()
{
	HRESULT hr;
	int returnVal = -1;
	IMbnRegistration *regInf = NULL;
	MBN_REGISTER_MODE regMode = MBN_REGISTER_MODE_MANUAL; //MBN_REGISTER_MODE_AUTOMATIC;
	ULONG regID = 0;
	BSTR proID = 0;

	//Get interface
	IMbnInterface *AdaptorInterface = GetAdaptorInterface();
	
	//Get registration interface
	hr = AdaptorInterface->QueryInterface(IID_IMbnRegistration,(void**)&regInf);
	if (FAILED(hr))
		returnVal = hr; 

	//Get Provider ID
	hr = regInf->GetProviderID(&proID);

	// Set Allowed GPRS speeds
	/*
	Third parameter
	A bitwise combination of OR MBN_DATA_CLASS values that specify the preferred data access technologies for the connection. 
	The Mobile Broadband service will register the highest available data class technology from this list. 
	If no data class from this list can be registered to, then the Mobile Broadband service will register to the best available data class.
	*/
		//Everything but 2g
	//~(MBN_DATA_CLASS_GPRS | MBN_DATA_CLASS_EDGE)
	
	hr = regInf->SetRegisterMode(regMode, /*NULL*/proID, MBN_DATA_CLASS_HSDPA | MBN_DATA_CLASS_HSUPA, &regID);//(MBN_DATA_CLASS_HSDPA | MBN_DATA_CLASS_HSUPA)|MBN_DATA_CLASS_UMTS,&regID);

	//clean up
	AdaptorInterface->Release();
	regInf->Release();
}
void SetGPRS_3GUMTS()
{
		HRESULT hr;
	int returnVal = -1;
	IMbnRegistration *regInf = NULL;
	MBN_REGISTER_MODE regMode = MBN_REGISTER_MODE_MANUAL;
	ULONG regID = 0;
	BSTR proID = 0;

	//Get interface
	IMbnInterface *AdaptorInterface = GetAdaptorInterface();
	
	//Get registration interface
	hr = AdaptorInterface->QueryInterface(IID_IMbnRegistration,(void**)&regInf);
	if (FAILED(hr))
		returnVal = hr; 

	//Get Provider ID
	hr = regInf->GetProviderID(&proID);

	// Set Allowed GPRS speeds
	/*
	Third parameter
	A bitwise combination of OR MBN_DATA_CLASS values that specify the preferred data access technologies for the connection. 
	The Mobile Broadband service will register the highest available data class technology from this list. 
	If no data class from this list can be registered to, then the Mobile Broadband service will register to the best available data class.
	*/
		//Everything but 2g
	//~(MBN_DATA_CLASS_GPRS | MBN_DATA_CLASS_EDGE)
	hr = regInf->SetRegisterMode(regMode, proID, MBN_DATA_CLASS_UMTS, &regID);//(MBN_DATA_CLASS_HSDPA | MBN_DATA_CLASS_HSUPA)|MBN_DATA_CLASS_UMTS,&regID);

	//clean up
	AdaptorInterface->Release();
	regInf->Release();
}
std::string GetCurrentMode()
{
	//MBN_REGISTER_MODE regMode = MBN_REGISTER_MODE_MANUAL;
	//ULONG regID =0;
	
	/* Set Mode by giving the flags of which data modes are allowed*/
	//hr = regInferface->SetRegisterMode(regMode, proID, MBN_DATA_CLASS_GPRS|MBN_DATA_CLASS_EDGE, &regID);

	MBN_DATA_CLASS mbnData;
	HRESULT hr;
	int returnVal = -1;
	IMbnRegistration *regInf = NULL;

	//Get interface
	IMbnInterface *AdaptorInterface = GetAdaptorInterface();
	
	//Get registration interface
	hr = AdaptorInterface->QueryInterface(IID_IMbnRegistration,(void**)&regInf);
	if (FAILED(hr))
		returnVal = hr; 

	//Get current data of mode
	hr = regInf->GetCurrentDataClass((ULONG*)&mbnData);
	if (FAILED(hr))
        returnVal = hr; 
	
	BSTR strName = 0;
	if(false)
	{
		
		hr = regInf->GetAvailableDataClasses((ULONG*)&mbnData);
		hr = regInf->GetRoamingText(&strName);

	}

	//unsigned int Mask = 0x1F;

	//Mask & mbnData



	if( mbnData == MBN_DATA_CLASS_GPRS)
	{
		std::cout<<"Data Speed: 2G"<<std::endl; return "2G";
	}

	if( mbnData == MBN_DATA_CLASS_EDGE)
	{
		std::cout<<"Data Speed: 2G"<<std::endl; return "2G";
	}

	if( mbnData == (MBN_DATA_CLASS_HSDPA | MBN_DATA_CLASS_HSUPA))
	{
		std::cout<<"Data Speed: 3G"<<std::endl; return "3G";
	}
	if(mbnData == MBN_DATA_CLASS_UMTS)
	{
		std::cout<<"Data Speed: 3G"<<std::endl; return "3G";
	}
	if( mbnData == (MBN_DATA_CLASS_HSDPA | MBN_DATA_CLASS_HSUPA))
	{
		std::cout<<"Data Speed: 3G"<<std::endl; return "3G";
	}
	if( mbnData == MBN_DATA_CLASS_UMTS)
	{
		std::cout<<"Data Speed: 3G"<<std::endl; return "3G";
	}


	// Clean up 
	regInf->Release();
	AdaptorInterface->Release();

	return "";

}
std::string GetSimState()
{
	MBN_READY_STATE SimState;

	int returnVal = -1;
	HRESULT hr = E_FAIL;

	//Get interface
	IMbnInterface *AdaptorInterface = GetAdaptorInterface();

	hr = AdaptorInterface->GetReadyState(&SimState);

	if (FAILED(hr))
		returnVal = hr; 
	//
	std::string strState ="";
	switch(SimState)
	{
		case MBN_READY_STATE_OFF             : 
			strState = "OFF";
			break;
		case MBN_READY_STATE_INITIALIZED     : 
			strState =  "INITIALISED";
			break;
		case MBN_READY_STATE_SIM_NOT_INSERTED: 
			strState = "SIM_NOT_INSERTED";
			break;
		case MBN_READY_STATE_BAD_SIM         : 
			strState = "BAD_SIM";
			break;
		case MBN_READY_STATE_FAILURE         : 
			strState = "FAILURE";
			break;
		case MBN_READY_STATE_NOT_ACTIVATED   : 
			strState = "NOT_ACTIVATED";
			break;
		case MBN_READY_STATE_DEVICE_LOCKED   : 
			strState = "DEVICE_LOCKED";
			break;
		case MBN_READY_STATE_DEVICE_BLOCKED  : 
			strState = "DEVICE_BLOCKED";
			break;
		default                              : 
			strState = "*** UNKNOWN ***";
			break;
	}
	std::cout<<"\nReady State: "+strState<<std::endl; 

	// Clean up
	AdaptorInterface->Release();

	return strState;
}
int GetSimPinINFO()
{
	IMbnPinManager *IpinManager = NULL;
	MBN_PIN_TYPE pinType = MBN_PIN_TYPE_PIN1;
	MBN_PIN_MODE pinMode = MBN_PIN_MODE_DISABLED;
	IMbnPin *Ipin = NULL;

	HRESULT hr = E_FAIL;

	//Get interface
	IMbnInterface * AdaptorInterface = GetAdaptorInterface();
	//Get pin interface
	AdaptorInterface->QueryInterface(IID_IMbnPinManager,(void**)&IpinManager);


	ULONG ulTest = 0;
	
	hr = IpinManager->GetPinState(&ulTest);
	hr = IpinManager->GetPin(pinType, (IMbnPin**)&Ipin);
	

	if(false)
	{

	//Enable pin
		hr = Ipin->Enable(L"1210", &ulTest);
	//Disble pin
		hr = Ipin->Disable(L"1210", &ulTest);
	// Change Pin
		hr = Ipin->Change(L"1210", L"1234", &ulTest);
	}

	hr = Ipin->get_PinMode(&pinMode);
	hr = Ipin->get_PinType(&pinType);

	ULONG ulID = 0;
	if(false)
		hr = Ipin->Enter(L"1234", &ulID);
	
	hr = Ipin->Enter(L"1210", &ulID);

	// Clean up
	IpinManager->Release();

	return 1;
}
std::string GetRegistrationState()
{
	IMbnRegistration *regInferface = NULL;
	IMbnInterface *AdaptorInterface = NULL;
	MBN_REGISTER_STATE regState = MBN_REGISTER_STATE_NONE;
	MBN_REGISTER_MODE regMode = MBN_REGISTER_MODE_NONE;
	ULONG DataClass;// = MBN_DATA_CLASS_NONE;
	ULONG DataClass2;
	ULONG packetAttachNetworkError;
	std::string strState = "";
	
	HRESULT hr = E_FAIL;
	int returnVal = -1;

	//Get interface
	AdaptorInterface = GetAdaptorInterface();

	hr = AdaptorInterface->QueryInterface(IID_IMbnRegistration,(void**)&regInferface);
	if (FAILED(hr))
		returnVal = hr; 

	regInferface->GetAvailableDataClasses(&DataClass);
	regInferface->GetCurrentDataClass(&DataClass2);
	regInferface->GetPacketAttachNetworkError(&packetAttachNetworkError);
	regInferface->GetRegisterMode(&regMode);
	regInferface->GetRegisterState(&regState);	


	switch(regState)
	{
		case MBN_REGISTER_STATE_NONE:
			strState = "Registed state: NONE";
			break;
		case MBN_REGISTER_STATE_DEREGISTERED:
			strState = "Registed state: DEREGISTERED";
			break;
		case MBN_REGISTER_STATE_SEARCHING:
			strState = "Registed state: SEARCHING";
			break;
		case MBN_REGISTER_STATE_HOME:
			strState = "Registed state: HOME";
			break;
		case MBN_REGISTER_STATE_ROAMING:
			strState = "Registed state: ROAMING";
			break;
		case MBN_REGISTER_STATE_PARTNER:
			strState = "Registed state: PARTNER";
			break;
		case MBN_REGISTER_STATE_DENIED:
			strState = "Registed state: DENIED";
			break;
	}

	//Get provider ID
	//BSTR proID = 0;
	//regInferface->GetProviderID(&proID);

	// Clean up
	regInferface->Release();
	AdaptorInterface->Release();

	return strState;

}
void GetDeviceInfo()
{
	MBN_INTERFACE_CAPS interfaceCapabilities;
	IMbnInterface *AdaptorInterface = NULL;

	HRESULT hr = E_FAIL;
	int returnVal = -1;

	//Get interface
	AdaptorInterface = GetAdaptorInterface();

	//Get Device Capabilities
	hr = AdaptorInterface->GetInterfaceCapability(&interfaceCapabilities);
	if (FAILED(hr))
		returnVal = hr;
			

	_tprintf(_T("cdma Band Class = %d\n"),  interfaceCapabilities.cdmaBandClass); 
	_tprintf(_T("cellular Class = %d\n"),  interfaceCapabilities.cellularClass); 
	_tprintf(_T("Mobile Broadband device control  = %d\n"),  interfaceCapabilities.controlCaps); 
	_tprintf(_T("Name of custom Band Class = %d\n"), interfaceCapabilities.customBandClass); 
	_tprintf(_T("Name ofcustom Data Class = %d\n"), interfaceCapabilities.customDataClass); 
	_tprintf(_T("Provider speed connection = %d\n"), interfaceCapabilities.dataClass);
	_tprintf(_T("Device ID: %s\n"), interfaceCapabilities.deviceID);
	_tprintf(_T("Firware Info: %s\n"), interfaceCapabilities.firmwareInfo);
	std::cout<<_bstr_t(interfaceCapabilities.firmwareInfo)<< std::endl;
	_tprintf(_T("gsm Band Class = %d\n"), interfaceCapabilities.gsmBandClass);
	_tprintf(_T("Manufactor: %s\n"), interfaceCapabilities.manufacturer);
	_tprintf(_T("Model: %s\n"), interfaceCapabilities.model);
	_tprintf(_T("sms Caps state = %d\n"), interfaceCapabilities.smsCaps);
	_tprintf(_T("Voice capabilities = %d\n"), interfaceCapabilities.voiceClass); 

	// Clean up
	AdaptorInterface->Release();
}
std::string GetProviderInfo()
{
	IMbnInterface *AdaptorInterface = NULL;
	IMbnSubscriberInformation *info = NULL;

	HRESULT hr = E_FAIL;
	int returnVal = -1;

	//The SIM International circuit card number
	BSTR SimIccID;
	// subscriber ID
	BSTR SubID;
	// telephone numbers associated with the device
	SAFEARRAY *Telephone_Numbers = NULL;

	//Get interface
	AdaptorInterface = GetAdaptorInterface();

	AdaptorInterface->GetSubscriberInformation(&info);
	
	info->get_SimIccID(&SimIccID);
	info->get_SubscriberID(&SubID);
				
	
	info->get_TelephoneNumbers(&Telephone_Numbers);

	AdaptorInterface->Release();
	info->Release();

	return "";
}
std::string GetSimIccID()
{
	IMbnInterface *AdaptorInterface = NULL;
	IMbnSubscriberInformation *ISubInfo = NULL;
	
	//The SIM International circuit card number
	BSTR SimIccID;

	//Get interface
	AdaptorInterface = GetAdaptorInterface();
	
	if(NULL == AdaptorInterface) return "";

	// Get the subcriber interface from the device interface
	AdaptorInterface->GetSubscriberInformation(&ISubInfo);

	ISubInfo->get_SimIccID(&SimIccID);

	//Clean up
	AdaptorInterface->Release();
	ISubInfo->Release();

	//return as string
	std::string  test = _bstr_t(SimIccID);
	return test;

}
std::string GetSubscriberID()
{
	IMbnInterface *AdaptorInterface = NULL;
	IMbnSubscriberInformation *ISubInfo = NULL;
	
	//The SIM International circuit card number
	BSTR SubscriberID;

	//Get interface
	AdaptorInterface = GetAdaptorInterface();

	if(NULL == AdaptorInterface) return "";
	
	// Get the subcriber interface from the device interface
	AdaptorInterface->GetSubscriberInformation(&ISubInfo);

	ISubInfo->get_SubscriberID(&SubscriberID);

	//Clean up
	AdaptorInterface->Release();
	ISubInfo->Release();

	//return as string
	std::string  test = _bstr_t(SubscriberID);
	return test;
}
bool EnableFullFunctionality()
{
	IMbnInterface *AdaptorInterface = NULL;
	IMbnRadio *RadioInf = NULL;
	MBN_RADIO RadioStatus = MBN_RADIO_OFF; 
	ULONG RequestID = 0;

	HRESULT hr = E_FAIL;
	int returnVal = -1;

	//Get interface
	AdaptorInterface = GetAdaptorInterface();
	if(NULL == AdaptorInterface) return false;

	AdaptorInterface->QueryInterface(IID_IMbnRadio, (void **)&RadioInf);

	// Hardware
	hr = RadioInf->get_HardwareRadioState(&RadioStatus);
	// Software
	hr = RadioInf->get_SoftwareRadioState(&RadioStatus);
	if (false)
	{
		hr = RadioInf->SetSoftwareRadioState(MBN_RADIO_OFF, &RequestID);
	}
	if (RadioStatus == MBN_RADIO_OFF )
	{
		hr = RadioInf->SetSoftwareRadioState(MBN_RADIO_ON, &RequestID);
			/// wait 

		ResetEvent(RadioEv->DeviceEnabled);
		

		WaitForSingleObject(RadioEv->DeviceEnabled, INFINITE);

		
		
		// If a sim is not inserted we will never get a new connection added, Need to query the device interface on the current sim status
		//std::string status = GetSimState();
		//if (status == "INITIALISED"){
		//	ResetEvent(ConnectionMgrEv->ConnectionAdded);
		//	WaitForSingleObject(ConnectionMgrEv->ConnectionAdded, INFINITE);
		//}

	}



	//clean up
	AdaptorInterface->Release();
	RadioInf->Release();

	return true;
}
void CreateXMLProfile()
{
	IMbnInterface *AdaptorInterface = NULL;
	SAFEARRAY *psaConPro = NULL;
	HRESULT hr = E_FAIL;
	int returnVal = -1;
	BSTR profileData;

	//Get interface
	AdaptorInterface = GetAdaptorInterface();

	if(NULL == AdaptorInterface) return;

	//delete profiles

	
	//if(false) /* NO APN TEST*/
	/*{
		    //Do the below each time(do not cache functional objects)
	std::string stringProfile = "<MBNProfile xmlns=\"http://www.microsoft.com/networking/WWAN/profile/v1\">\
								<Name>EBITS_MOBILE_BROADBAND_CONNECTION_PROFILE</Name>\
							  <IsDefault>true</IsDefault>\
							  <SubscriberID>"+GetSubscriberID()+"</SubscriberID>\
							  <SimIccID>"+GetSimIccID()+"</SimIccID>\
							  <AutoConnectOnInternet>true</AutoConnectOnInternet>\
							  <ConnectionMode>auto</ConnectionMode>\
							  <Context>\
								<AccessString></AccessString>\
								<Compression>ENABLE</Compression>\
								<AuthProtocol>CHAP</AuthProtocol>\
							  </Context>\
							</MBNProfile>";

	}*/


    //Do the below each time(do not cache functional objects)
	std::string stringProfile = "<MBNProfile xmlns=\"http://www.microsoft.com/networking/WWAN/profile/v1\">\
								<Name>EBITS_MOBILE_BROADBAND_PROFILE</Name>\
							  <IsDefault>true</IsDefault>\
							  <SubscriberID>"+GetSubscriberID()+"</SubscriberID>\
							  <SimIccID>"+GetSimIccID()+"</SimIccID>\
							  <AutoConnectOnInternet>true</AutoConnectOnInternet>\
							  <ConnectionMode>auto</ConnectionMode>\
							  <Context>\
								<AccessString>general.t-mobile.uk</AccessString>\
								<Compression>ENABLE</Compression>\
								<AuthProtocol>CHAP</AuthProtocol>\
							  </Context>\
							</MBNProfile>";
	//general.t-mobile.uk
	std::wstring stemp = std::wstring(stringProfile.begin(), stringProfile.end());
	LPCWSTR xmlProfile = stemp.c_str();

	hr = g_ConnectionProfileMgr->CreateConnectionProfile(xmlProfile);
    if (FAILED(hr))
        returnVal = hr; 
	else
	{
		WaitForSingleObject(ConnectionProfileMgrEv->ProfileAdded, INFINITE);
	}
		//Dont break as we may have created one 
	//Issue here where when profile created it does not allow me to get the connection profile
	hr = g_ConnectionProfileMgr->GetConnectionProfiles(AdaptorInterface, &psaConPro);
    if (FAILED(hr))
        returnVal = hr; 

    LONG lLower3;
    LONG lUpper3;

    hr = SafeArrayGetLBound(psaConPro, 1, &lLower3);
    if (FAILED(hr))
        returnVal = hr; 

    hr = SafeArrayGetUBound(psaConPro, 1, &lUpper3);
    if (FAILED(hr))
        returnVal = hr; 

	IMbnConnectionProfile *ConnectionProfile = NULL;
	for (LONG l = lLower3; l <= lUpper3; l++)
    {
		
		hr = SafeArrayGetElement(psaConPro, &l, (void*)(&ConnectionProfile));

		ConnectionProfile->GetProfileXmlData(&profileData);
		//delete profiles
		if(false)
			ConnectionProfile->Delete();
		////
	}


	// Clean up
	AdaptorInterface->Release();
}
bool Connect()
{
	// Get the connection interface IMbnConnection
	IMbnConnection *pConnectionInf = NULL;
	IMbnConnectionProfile *connProfile =  NULL;
	MBN_CONNECTION_MODE Mode = /*MBN_CONNECTION_MODE_TMP_PROFILE;*/MBN_CONNECTION_MODE_PROFILE;//
	ULONG RequestId = 0;
	BSTR xmlProfile;
	HRESULT hr = E_FAIL;
	int returnVal = -1;
	LONG lLower2;
    LONG lUpper2;
	SAFEARRAY *psaCon = NULL;



	//Do the below each time(do not cache functional objects)
	hr = g_ConnectionMgr->GetConnections(&psaCon);
	if (FAILED(hr))
		returnVal = hr; 


    hr = SafeArrayGetLBound(psaCon, 1, &lLower2);
    if (FAILED(hr))
        returnVal = hr; 

    hr = SafeArrayGetUBound(psaCon, 1, &lUpper2);
    if (FAILED(hr))
        returnVal = hr; 

	for (LONG l = lLower2; l <= lUpper2; l++)
	{
		hr = SafeArrayGetElement(psaCon, &l, (void*)(&pConnectionInf));
		if (FAILED(hr))
			returnVal = hr;
	}

	// Get Ebits profile
	hr = g_ConnectionProfileMgr->GetConnectionProfile(GetAdaptorInterface(),L"EBITS_MOBILE_BROADBAND_PROFILE",&connProfile);
	if(FAILED(hr))
		returnVal = hr;

	// Get the XML data
	connProfile->GetProfileXmlData(&xmlProfile);

	//Connect using the xml profile, mode and returns asynchronous request ID
	hr = pConnectionInf->Connect(Mode, L"EBITS_MOBILE_BROADBAND_PROFILE", &RequestId);
	if(FAILED(hr))
		//E_MBN_MAX_ACTIVATED_CONTEXTS Means we have a active connection
		returnVal = hr; 

	ULONG lTest = 0;

	pConnectionInf->GetActivationNetworkError(&lTest);

	// Store connection ID
	pConnectionInf->get_ConnectionID((BSTR*)&intConnectionID);

	// Clean up
	pConnectionInf->Release();
	connProfile->Release();

	return true;
}
boolean Disconnect()
{
	HRESULT hr = E_FAIL;
	IMbnConnection *ConnectionInterface = NULL;

	ConnectionInterface = GetConnectionInterface();
	if(NULL != ConnectionInterface)
	{
		// Pointer to unique request ID assigned by the Mobile Broadband service to identify this asynchronous request.
		ULONG ulRequestID = 0;
		hr = ConnectionInterface->Disconnect(&ulRequestID);

		// Clean up
		ConnectionInterface->Release();
	}

	return true;
}
bool SetMobileBroadbandInterfaceID()
{
	SAFEARRAY *psa = NULL;
	IMbnInterface *AdaptorInterface = NULL;
	HRESULT hr = E_FAIL;
	int returnVal = -1;
	LONG lLower;
    LONG lUpper;

    //Do the below each time(do not cache functional objects)
    hr = g_InterfaceMgr->GetInterfaces(&psa);
    if (FAILED(hr))
        returnVal = hr;

    hr = SafeArrayGetLBound(psa, 1, &lLower);
    if (FAILED(hr))
        returnVal = hr; 

    hr = SafeArrayGetUBound(psa, 1, &lUpper);
    if (FAILED(hr))
        returnVal = hr; 

    for (LONG l = lLower; l <= lUpper; l++)
    {

        hr = SafeArrayGetElement(psa, &l, (void*)(&AdaptorInterface));
        if (FAILED(hr))
			returnVal = hr; 
		/* 
		We assume that the Nexcon unit will only hae one interface as you
		have one interface to every Mobile Broadband adaptor 
		*/
		if(NULL != AdaptorInterface) AdaptorInterface->get_InterfaceID((BSTR*)&intInterfaceID);
    }
	//Clean up
	if(NULL != AdaptorInterface)  AdaptorInterface->Release();

	return true;
}
void setconID()
{
	HRESULT hr = E_FAIL;	
	IMbnConnection  *ConnectionInf = NULL;
	SAFEARRAY *AdapterConnections = NULL;

	hr = g_ConnectionMgr->GetConnections(&AdapterConnections);
	if(SUCCEEDED(hr))
	{
		LONG lLower;
		LONG lUpper;

		SafeArrayGetLBound(AdapterConnections, 1, &lLower);


		SafeArrayGetUBound(AdapterConnections, 1, &lUpper);
		 /* 
			We assume that the Nexcon unit will only have one GPRS connection as you
			have one connection to every Mobile Broadband adapter 
		*/   
		for (LONG l = lLower; l <= lUpper; l++)
			SafeArrayGetElement(AdapterConnections, &l, (void*)(&ConnectionInf));

		// Set Connection ID
	
		hr = ConnectionInf->get_ConnectionID((BSTR*)&intConnectionID);
	}

	// Clean up
	if(ConnectionInf)ConnectionInf->Release();
	if (AdapterConnections) 
	{
		SafeArrayDestroy(AdapterConnections);
		AdapterConnections = NULL;
	}
}
bool SetInterfaceMgr()
{
	HRESULT hr = E_FAIL;
	int returnVal = -1;
	//Do the below once(cache the manager objects)

	//IMbnInterfaceManager *g_InterfaceMgr = new IMbnInterfaceManager();

	hr = CoCreateInstance(CLSID_MbnInterfaceManager,
	    NULL, 
	    CLSCTX_ALL, 
	    IID_IMbnInterfaceManager, 
	    (void**)&g_InterfaceMgr);
	if (FAILED(hr))
	    returnVal = hr;

	return true;

}
bool SetConnectionProfileMgr()
{
	HRESULT hr = E_FAIL;
	int returnVal = -1;

	//CComPtr<IMbnInterfaceManager>  g_InterfaceMgr = NULL;
	//Do the below once(cache the manager objects)
	hr = CoCreateInstance(CLSID_MbnConnectionProfileManager,
										NULL, 
										CLSCTX_ALL, 
										IID_IMbnConnectionProfileManager, 
										(void**)&g_ConnectionProfileMgr);
	if (FAILED(hr))
		returnVal = hr; 

	return true;
}
bool SetConnectionMgr()
{
	HRESULT hr = E_FAIL;
	int returnVal = -1;
	//Do the below once(cache the manager objects)
	hr = CoCreateInstance(CLSID_MbnConnectionManager,
										NULL, 
										CLSCTX_ALL, 
										IID_IMbnConnectionManager, 
										(void**)&g_ConnectionMgr);
	if (FAILED(hr))
		returnVal = hr; 

	return true;
}
void AutoConnectProfileUpdate()
{
	HRESULT hr = E_FAIL;
	int returnVal = -1;
	// Get Ebits profile
	IMbnConnectionProfile *connProfile =  NULL;

	hr = g_ConnectionProfileMgr->GetConnectionProfile(GetAdaptorInterface(),L"EBITS_MOBILE_BROADBAND_CONNECTION_PROFILE",&connProfile);
	if(FAILED(hr))
		returnVal = hr;

		std::string stringProfile = "<MBNProfile xmlns=\"http://www.microsoft.com/networking/WWAN/profile/v1\">\
							  <Name>EBITS_MOBILE_BROADBAND_CONNECTION_PROFILE</Name>\
							  <IsDefault>true</IsDefault>\
							  <SubscriberID>"+GetSubscriberID()+"</SubscriberID>\
							  <SimIccID>"+GetSimIccID()+"</SimIccID>\
							  <AutoConnectOnInternet>true</AutoConnectOnInternet>\
							  <ConnectionMode>auto</ConnectionMode>\
							  <Context>\
								<AccessString>general.t-mobile.uk</AccessString>\
								<Compression>ENABLE</Compression>\
								<AuthProtocol>CHAP</AuthProtocol>\
							  </Context>\
							</MBNProfile>";

	std::wstring stemp = std::wstring(stringProfile.begin(), stringProfile.end());
	LPCWSTR xmlProfile = stemp.c_str();

	hr = connProfile->UpdateProfile(xmlProfile);
	WaitForSingleObject(ConnectionProfileEv->ProfileUpdated, INFINITE);

	connProfile->Release();
}
void ManualConnectProfileUpdate()
{
	HRESULT hr = E_FAIL;
	int returnVal = -1;
	// Get Ebits profile
	IMbnConnectionProfile *connProfile =  NULL;

	hr = g_ConnectionProfileMgr->GetConnectionProfile(GetAdaptorInterface(),L"EBITS_MOBILE_BROADBAND_PROFILE",&connProfile);
	if(FAILED(hr))
		returnVal = hr;

		std::string stringProfile = "<MBNProfile xmlns=\"http://www.microsoft.com/networking/WWAN/profile/v1\">\
							  <Name>EBITS_MOBILE_BROADBAND_PROFILE</Name>\
							  <IsDefault>true</IsDefault>\
							  <SubscriberID>"+GetSubscriberID()+"</SubscriberID>\
							  <SimIccID>"+GetSimIccID()+"</SimIccID>\
							  <AutoConnectOnInternet>true</AutoConnectOnInternet>\
							  <ConnectionMode>manual</ConnectionMode>\
							  <Context>\
								<AccessString>general.t-mobile.uk</AccessString>\
								<Compression>ENABLE</Compression>\
								<AuthProtocol>CHAP</AuthProtocol>\
							  </Context>\
							</MBNProfile>";

	std::wstring stemp = std::wstring(stringProfile.begin(), stringProfile.end());
	LPCWSTR xmlProfile = stemp.c_str();

	hr = connProfile->UpdateProfile(xmlProfile);
	WaitForSingleObject(ConnectionProfileEv->ProfileUpdated, INFINITE);

	connProfile->Release();
}
void AutoConnectionProfileUpdateNotRoaming()
{
		HRESULT hr = E_FAIL;
	int returnVal = -1;
	// Get Ebits profile
	IMbnConnectionProfile *connProfile =  NULL;

	hr = g_ConnectionProfileMgr->GetConnectionProfile(GetAdaptorInterface(),L"EBITS_MOBILE_BROADBAND_PROFILE",&connProfile);
	if(FAILED(hr))
		returnVal = hr;

		std::string stringProfile = "<MBNProfile xmlns=\"http://www.microsoft.com/networking/WWAN/profile/v1\">\
							  <Name>EBITS_MOBILE_BROADBAND_PROFILE</Name>\
							  <IsDefault>true</IsDefault>\
							  <SubscriberID>"+GetSubscriberID()+"</SubscriberID>\
							  <SimIccID>"+GetSimIccID()+"</SimIccID>\
							  <AutoConnectOnInternet>true</AutoConnectOnInternet>\
							  <ConnectionMode>auto-home</ConnectionMode>\
							  <Context>\
								<AccessString>general.t-mobile.uk</AccessString>\
								<Compression>ENABLE</Compression>\
								<AuthProtocol>CHAP</AuthProtocol>\
							  </Context>\
							</MBNProfile>";

	std::wstring stemp = std::wstring(stringProfile.begin(), stringProfile.end());
	LPCWSTR xmlProfile = stemp.c_str();

	hr = connProfile->UpdateProfile(xmlProfile);
	WaitForSingleObject(ConnectionProfileEv->ProfileUpdated, INFINITE);

	connProfile->Release();
}
void AutoConnectionProfileUpdateNotRoamingWithPinUsername()
{
		HRESULT hr = E_FAIL;
	int returnVal = -1;
	// Get Ebits profile
	IMbnConnectionProfile *connProfile =  NULL;

	hr = g_ConnectionProfileMgr->GetConnectionProfile(GetAdaptorInterface(),L"EBITS_MOBILE_BROADBAND_PROFILE",&connProfile);
	if(FAILED(hr))
		returnVal = hr;

		std::string stringProfile = "<MBNProfile xmlns=\"http://www.microsoft.com/networking/WWAN/profile/v1\">\
							  <Name>EBITS_MOBILE_BROADBAND_PROFILE</Name>\
							  <IsDefault>true</IsDefault>\
							  <SubscriberID>"+GetSubscriberID()+"</SubscriberID>\
							  <SimIccID>"+GetSimIccID()+"</SimIccID>\
							  <AutoConnectOnInternet>true</AutoConnectOnInternet>\
							  <ConnectionMode>auto-home</ConnectionMode>\
							  <Context>\
								<AccessString>general.t-mobile.uk</AccessString>\
								<UserLogonCred>\
									<UserName>user</UserName>\
									<Password>1210</Password>\
								</UserLogonCred>\
								<Compression>ENABLE</Compression>\
								<AuthProtocol>NONE</AuthProtocol>\
							  </Context>\
							</MBNProfile>";


		/*								<UserLogonCred>\
									<UserName></UserName>\
									<Password></Password>\
									<IgnorePassword>false</IgnorePassword>\
								</UserLogonCred>\*/
	std::wstring stemp = std::wstring(stringProfile.begin(), stringProfile.end());
	LPCWSTR xmlProfile = stemp.c_str();

	hr = connProfile->UpdateProfile(xmlProfile);

	connProfile->Release();
}

void getConnectionState()
{
	MBN_ACTIVATION_STATE state = MBN_ACTIVATION_STATE_NONE;
	IMbnConnection *ConnectionInf = NULL;
	BSTR ProfileName;
	HRESULT hr;

	g_ConnectionMgr->GetConnection(intConnectionID, &ConnectionInf);
	hr = ConnectionInf->GetConnectionState(&state, &ProfileName);
	if(SUCCEEDED(hr))
	{
		switch(state)
		{
			case MBN_ACTIVATION_STATE_NONE:std::cout<<"The connection state is unknown.\n"; break;
			case MBN_ACTIVATION_STATE_ACTIVATED:std::cout<<"The connection has been established.\n"; break;
			case MBN_ACTIVATION_STATE_ACTIVATING:std::cout<<"The device is establishing the connection.\n"; break;
			case MBN_ACTIVATION_STATE_DEACTIVATED:std::cout<<"There is no connection.\n"; break;
			case MBN_ACTIVATION_STATE_DEACTIVATING:std::cout<<"The device is in the process of disconnection.\n"; break;

			default: 
				std::cout<<"FAILED\n";
		}
	}


}

void GetVisableProviders()
{
	HRESULT hr = E_FAIL;
	int returnVal = -1;
	SAFEARRAY *Providers = NULL;
	ULONG age;
	ULONG ID;
	IMbnInterface *Deviceinterface = GetAdaptorInterface();


	hr = Deviceinterface->ScanNetwork(&ID);
	ResetEvent(InterfaceEv->evScanComplete);
	WaitForSingleObject(InterfaceEv->evScanComplete, INFINITE);


	hr = Deviceinterface->GetVisibleProviders(&age,&Providers);


	LONG lLower;
	LONG lUpper;

	MBN_PROVIDER PROVIDER;

	if (FAILED(hr))
		returnVal = hr;

	hr = SafeArrayGetLBound(Providers, 1, &lLower);
	if (FAILED(hr))
		returnVal = hr; 

	hr = SafeArrayGetUBound(Providers, 1, &lUpper);
	if (FAILED(hr))
		returnVal = hr; 

	for (LONG l = lLower; l <= lUpper; l++)
	{
		//LPCWSTR ConnectionID;
		hr = SafeArrayGetElement(Providers, &l, (void*)(&PROVIDER));
		if (FAILED(hr))
			returnVal = hr; 

		//ConnectionInterface->get_ConnectionID((BSTR*)ConnectionID);
	}

}

void printNetConDetails(NETCON_PROPERTIES &pProps)
{
	std::cout<<"\n----- ----- ----- ----- ----- ----- ----- ----- -----\n";
	std::cout<<"Name : "+ toString(pProps.pszwName)<<std::endl; 
	std::cout<<"Device Name : "+ toString(pProps.pszwDeviceName)<<std::endl;

	std::cout<<"Status : ";
	switch (pProps.Status)
	{
	case NCS_DISCONNECTED :	std::cout<<"NCS_DISCONNECTED"<<std::endl; break;
	case NCS_CONNECTING :	std::cout<<"NCS_CONNECTING"<<std::endl; break;
	case NCS_CONNECTED  :	std::cout<<"NCS_CONNECTED"<<std::endl; break;
	case NCS_DISCONNECTING  :	std::cout<<"NCS_DISCONNECTING"<<std::endl; break;
	case NCS_HARDWARE_NOT_PRESENT  :	std::cout<<"NCS_HARDWARE_NOT_PRESENT"<<std::endl; break;
	case NCS_HARDWARE_DISABLED  :	std::cout<<"NCS_HARDWARE_DISABLED"<<std::endl; break;
	case NCS_HARDWARE_MALFUNCTION  :	std::cout<<"NCS_HARDWARE_MALFUNCTION"<<std::endl; break;
	case NCS_MEDIA_DISCONNECTED  :	std::cout<<"NCS_MEDIA_DISCONNECTED"<<std::endl; break;
	case NCS_AUTHENTICATING  :	std::cout<<"NCS_AUTHENTICATING"<<std::endl; break;
	case NCS_AUTHENTICATION_SUCCEEDED :	std::cout<<"NCS_AUTHENTICATION_SUCCEEDED"<<std::endl; break;
	case NCS_AUTHENTICATION_FAILED :	std::cout<<"NCS_AUTHENTICATION_FAILED"<<std::endl; break;
	case NCS_INVALID_ADDRESS :	std::cout<<"NCS_INVALID_ADDRESS"<<std::endl; break;
	case NCS_CREDENTIALS_REQUIRED :	std::cout<<"NCS_CREDENTIALS_REQUIRED"<<std::endl; break;
	}

	std::cout<<"Media Type : ";
	switch (pProps.Status)
	{
	case NCM_NONE :	std::cout<<"NCM_NONE"<<std::endl; break;
	case NCM_DIRECT :	std::cout<<"NCM_DIRECT"<<std::endl; break;
	case NCM_ISDN  :	std::cout<<"NCM_ISDN"<<std::endl; break;
	case NCM_LAN  :	std::cout<<"NCM_LAN"<<std::endl; break;
	case NCM_PHONE  :	std::cout<<"NCM_PHONE"<<std::endl; break;
	case NCM_TUNNEL  :	std::cout<<"NCM_TUNNEL"<<std::endl; break;
	case NCM_PPPOE  :	std::cout<<"NCM_PPPOE"<<std::endl; break;
	case NCM_BRIDGE  :	std::cout<<"NCM_BRIDGE"<<std::endl; break;
	case NCM_SHAREDACCESSHOST_LAN  :	std::cout<<"NCM_SHAREDACCESSHOST_LAN"<<std::endl; break;
	case NCM_SHAREDACCESSHOST_RAS :	std::cout<<"NCM_SHAREDACCESSHOST_RAS"<<std::endl; break;
	}


	std::cout<<"\n----- ----- ----- ----- ----- ----- ----- ----- -----\n";
}


void setState(const boolean newState, std::string adapterName)
{
	bool blnResult = false;

	INetConnectionManager *pNetConnectionManager = NULL;
	HRESULT hr = CoCreateInstance(	CLSID_ConnectionManager,
  	                        		NULL,
   	                       			CLSCTX_LOCAL_SERVER | CLSCTX_NO_CODE_DOWNLOAD,
    	                      		IID_INetConnectionManager,
     	                     		reinterpret_cast<LPVOID *>(&pNetConnectionManager)
      	                   		);
   	if(SUCCEEDED(hr))
   	{
		//Get an enumurator for the set of connections on the system
       	IEnumNetConnection* pEnumNetConnection = NULL;

		HRESULT Test = pNetConnectionManager->EnumConnections(NCME_DEFAULT, &pEnumNetConnection);

       	if(SUCCEEDED(pNetConnectionManager->EnumConnections(NCME_DEFAULT, &pEnumNetConnection)) && (NULL != pEnumNetConnection))
       	{
        	ULONG ulCount = 0;

			//Enumurate through the list of adapters on the system and look for the one we want   		
        	do
        	{
         	   	NETCON_PROPERTIES*	pProps = NULL;
          	  	INetConnection*		pConn = NULL;

   				//Find the next (or first connection)
            	hr = pEnumNetConnection->Next(1, &pConn, &ulCount);

            	if (SUCCEEDED(hr) && 1 == ulCount && (NULL != pConn))
            	{
					//Get the connection properties
                	HRESULT hrt = pConn->GetProperties(&pProps);
    				if (S_OK == hrt && (NULL != pProps))
                	{
                		//Convert the adapter GUID to an OMString                	
						if( (toString(pProps->pszwName)).find(adapterName) != std::string::npos)
                		{
							std::cout<<"\nFound\n";
               				HRESULT hrc = (newState ? pConn->Connect() : pConn->Disconnect()); 		
							blnResult = (hrc == S_OK);
							printNetConDetails(*pProps);
							break;
						}
						
						CoTaskMemFree (pProps->pszwName);
                        CoTaskMemFree (pProps->pszwDeviceName);
                    	CoTaskMemFree (pProps);
					}	
					
					pConn->Release();               	             
            	}

        	}while(SUCCEEDED(hr) && 1 == ulCount);

        	pEnumNetConnection->Release();
        }
        
        pNetConnectionManager->Release();       
	}
	if(!blnResult) std::cout<<"\nCould not find '" + adapterName + "'\n";
}

void GetAdapterINFO()
{
	bool blnResult = false;

	INetConnectionManager *pNetConnectionManager = NULL;
	HRESULT hr = CoCreateInstance(	CLSID_ConnectionManager,
  	                        		NULL,
   	                       			CLSCTX_LOCAL_SERVER | CLSCTX_NO_CODE_DOWNLOAD,
    	                      		IID_INetConnectionManager,
     	                     		reinterpret_cast<LPVOID *>(&pNetConnectionManager)
      	                   		);
   	if(SUCCEEDED(hr))
   	{
		//Get an enumurator for the set of connections on the system
       	IEnumNetConnection* pEnumNetConnection = NULL;

		HRESULT Test = pNetConnectionManager->EnumConnections(NCME_DEFAULT, &pEnumNetConnection);

       	if(SUCCEEDED(pNetConnectionManager->EnumConnections(NCME_DEFAULT, &pEnumNetConnection)) && (NULL != pEnumNetConnection))
       	{
        	ULONG ulCount = 0;

			//Enumurate through the list of adapters on the system and look for the one we want   		
        	do
        	{
         	   	NETCON_PROPERTIES*	pProps = NULL;
          	  	INetConnection*		pConn = NULL;

   				//Find the next (or first connection)
            	hr = pEnumNetConnection->Next(1, &pConn, &ulCount);

            	if (SUCCEEDED(hr) && 1 == ulCount && (NULL != pConn))
            	{
					//Get the connection properties
                	HRESULT hrt = pConn->GetProperties(&pProps);
    				if (S_OK == hrt && (NULL != pProps))
                	{
                		//Convert the adapter GUID to an OMString                	
                		//if(m_strFriendlyName == clsStringManipulator::FromUnicodeString(pProps->pszwName))
                		{
               				//HRESULT hrc = (NewState == DICS_ENABLE) ? pConn->Connect() : pConn->Disconnect();  			
							//HRESULT hrc = pConn->Disconnect();  			
							//blnResult = (hrc == S_OK);
						}
						
						printNetConDetails(*pProps);
						
						CoTaskMemFree (pProps->pszwName);
                        CoTaskMemFree (pProps->pszwDeviceName);
                    	CoTaskMemFree (pProps);
					}	
					
					pConn->Release();               	             
            	}

        	}while(SUCCEEDED(hr) && 1 == ulCount);

        	pEnumNetConnection->Release();
        }
        
        pNetConnectionManager->Release();       
	}
}

void disableAdapter()
{
	std::string deviceName = "";
	std::cout<<"\nEnter device Name to be diabled : ";
	std::cin>>deviceName;

	setState(false, deviceName);
}

void enableAdapter()
{
	std::string deviceName = "";
	std::cout<<"\nEnter Name to be enabled : ";
	std::cin>>deviceName;

	setState(true, deviceName);
}


int main()
{

		HINTERNET hOpen;                       // Root HINTERNET handle
	INTERNET_STATUS_CALLBACK iscCallback;  // Holds the callback function
	int i = 0;

	// Create the root HINTERNET handle.
	hOpen = InternetOpen( TEXT("Mobile Broadband API"),
					INTERNET_OPEN_TYPE_PRECONFIG,
					NULL, NULL, 0);

	// Set the status callback function.
	iscCallback = InternetSetStatusCallback( hOpen, (INTERNET_STATUS_CALLBACK)CallMaster );


    HRESULT hr = E_FAIL;
    int returnVal = 0;

	BSTR ProID = L"-1";

    do
    {
        hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);    
        if (FAILED(hr))
        {
            returnVal = hr; 
            break;
        }




		SetInterfaceMgr();
		SetConnectionMgr();
		SetConnectionProfileMgr();

		SetEvents();

		SetMobileBroadbandInterfaceID();

		EnableFullFunctionality();

		setconID();

		




		CreateXMLProfile();

		int i = -1;
		while (i != 0)
		{
			std::cout<<"Enable 2G: 1\n"; 
			std::cout<<"Enable 3G: 2\n";
			std::cout<<"Connect:   3\n";
			std::cout<<"Disconnect:4\n";
			std::cout<<"Update profile with Auto Connect:5\n";
			std::cout<<"Update profile with Manaual Connect:6\n";
			std::cout<<"Update profile with Auto Connect non-roaming:7\n";
			std::cout<<"Show current Mode:8\n";
			std::cout<<"Enable 3G UMTS: 9\n";
			std::cout<<"Show device info: 10\n";
			std::cout<<"Show Pin info: 11\n";
			std::cout<<"Update profile with Auto Connect non-roaming with username and pass: 12\n";
			std::cout<<"Enable Full Fucntionality: 13\n";
			std::cout<<"Get Registration State: 14\n";
			std::cout<<"Get Connection State: 15\n";
			std::cout<<"Get Visable Providers: 16\n";
			std::cout<<"Get Adapter INFO: 17\n";
			std::cout<<"Disable Adapter : 18\n";
			std::cout<<"Enable Adapter : 19\n";
			std::cout<<"Exit:      0\n";
			

			std::cin>>i;

			switch(i)
			{
				case 1:SetGPRS_2G();
					break;
				case 2:SetGPRS_3G_Full();
					break;
				case 3:Connect();
					break;
				case 4:Disconnect();
					break;
				case 5:AutoConnectProfileUpdate();
					break;
				case 6:ManualConnectProfileUpdate();
					break;
				case 7:AutoConnectionProfileUpdateNotRoaming();
					break;
				case 8:GetCurrentMode();
					break;
				case 9:
					{
						SetGPRS_3GUMTS();
						GetCurrentMode();
					}
					break;
				case 10:GetDeviceInfo();
					break;
				case 11:GetSimPinINFO();
					break;
				case 12:AutoConnectionProfileUpdateNotRoamingWithPinUsername();
					break;
				case 13:EnableFullFunctionality();
					break;
				case 14:GetRegistrationState();
					break;
				case 15:getConnectionState();
					break;
				case 16:GetVisableProviders();
					break;
				case 17:GetAdapterINFO();
					break;
				case 18:disableAdapter();
					break;
				case 19:enableAdapter();
					break;
			}

			i = -1;
		}



		//std::cout<<"Connect for 15 seconds and disconnect"<<std::endl;
		//if(!Connect())
			//int i = 0;

		//Sleep(15000);

		//SetDeviceInfo();

		//Disconnect();

		//std::cout<<"Disconnect"<<std::endl;
		//Sleep(3000);


    } while (FALSE);


    CoUninitialize();
    return returnVal;
}

