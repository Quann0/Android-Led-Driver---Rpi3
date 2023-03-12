/*
 * Copyright (C) 2012 Opersys inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.os;

import android.os.IOpersysService;
import android.content.Context;
import android.util.Log;

public class OpersysManager
{
    public String read(int maxLength) {
    	Log.e("Quan", "OpersysManager String read location1\n");
	try {
	    return mService.read(maxLength);
	} catch (RemoteException e) {
	    return null;
	}
    }

    public int write(String mString) {
    	Log.e("Quan", "OpersysManager String write location1\n");
	try {
	    return mService.write(mString);
	} catch (RemoteException e) {
	    return 0;
	}
    }

    public OpersysManager(IOpersysService service) {
        mService = service;
    }

    IOpersysService mService;
}
