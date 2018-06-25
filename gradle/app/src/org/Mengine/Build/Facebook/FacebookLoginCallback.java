package org.Mengine.Build.Facebook;

import com.facebook.FacebookException;
import com.facebook.login.LoginResult;

/**
 * Created by sweatcoin7 on 6/25/18.
 */

public interface FacebookLoginCallback {
    void onLoginSuccess(FacebookLoginResultWrapper loginResult);
    void onLoginCancel();
    void onLoginError(FacebookException exception);
}
