/*************************************************************************/
/*  camera_win.h                                                         */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef CAMERAWIN_H
#define CAMERAWIN_H

#include "servers/camera/camera_feed.h"
#include "servers/camera_server.h"

#include <comdef.h>

// Media Foundation Includes
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

//Some versions of the windows api apparently don't have this, so I am creating it manualy
EXTERN_GUID(CUSTOM_MF_READWRITE_DISABLE_CONVERTERS, 0x98d5b065, 0x1374, 0x4847, 0x8d, 0x5d, 0x31, 0x52, 0x0f, 0xee, 0x71, 0x56);

#include <shlwapi.h>

class CameraFeedWindows : public CameraFeed, public IMFSourceReaderCallback {
private:
	// using ms samples naming
	IMFActivate *pActivate;
	HRESULT CloseDevice();

	// device.h
	UINT m_width;
	UINT m_height;
	const UINT m_depth = 4;
	LONG m_lDefaultStride;
	MFRatio m_PixelAR;
	MFVideoInterlaceMode m_interlace;

	PoolVector<uint8_t> img_data;

protected:
	void NotifyError(HRESULT hr);
	long m_nRefCount; // Reference count.
	CRITICAL_SECTION m_critsec;

	IMFSourceReader *m_pReader;
	WCHAR *m_pwszSymbolicLink;
	UINT32 m_cchSymbolicLink;

public:
	CameraFeedWindows();
	virtual ~CameraFeedWindows();

	void set_device(IMFActivate *p_device);

	bool activate_feed();
	void deactivate_feed();

	// virtual functions needed to be implemented for callbacks

	// IUnknown methods
	STDMETHODIMP QueryInterface(REFIID iid, void **ppv);
	STDMETHODIMP_(ULONG)
	AddRef();
	STDMETHODIMP_(ULONG)
	Release();

	// IMFSourceReaderCallback methods
	STDMETHODIMP OnReadSample(
			HRESULT hrStatus,
			DWORD dwStreamIndex,
			DWORD dwStreamFlags,
			LONGLONG llTimestamp,
			IMFSample *pSample);

	STDMETHODIMP OnEvent(DWORD, IMFMediaEvent *) {
		return S_OK;
	}

	STDMETHODIMP OnFlush(DWORD) {
		return S_OK;
	}
};

class CameraWindows : public CameraServer {
private:
	void add_active_cameras();

public:
	CameraWindows();
	~CameraWindows();
};

#endif /* CAMERAWIN_H */
