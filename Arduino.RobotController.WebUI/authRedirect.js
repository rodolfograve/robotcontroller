// Create the main myMSALObj instance
// configuration parameters are located at authConfig.js
const myMSALObj = new Msal.UserAgentApplication(msalConfig); 

let accessToken;

// Register Callbacks for Redirect flow
myMSALObj.handleRedirectCallback(authRedirectCallBack);

function authRedirectCallBack(error, response) {
  if (error) {
      console.log(error);
  } else {
      if (response.tokenType === "id_token") {
          console.log('id_token acquired at: ' + new Date().toString());
      } else if (response.tokenType === "access_token") {
          console.log('access_token acquired at: ' + new Date().toString());
          accessToken = response.accessToken;

          callMSGraph(graphConfig.graphMailEndpoint, accessToken, updateUI);
          profileButton.style.display = 'none';
          mailButton.style.display = 'initial';
      } else {
          console.log("token type is:" + response.tokenType);
      }
  }
}

// Redirect: once login is successful and redirects with tokens, call Graph API
if (myMSALObj.getAccount()) {
  onLoginSuccess(myMSALObj.getAccount());
}

function signIn() {
  myMSALObj.loginRedirect(msalConfig.loginRequest);
}

// This function can be removed if you do not need to support IE
function getTokenRedirect(request, endpoint) {
  return myMSALObj.acquireTokenSilent(request, endpoint)
      .then((response) => {
          console.log(response);
          if (response.accessToken) {
              console.log('access_token acquired at: ' + new Date().toString());
              accessToken = response.accessToken;

              callMSGraph(endpoint, response.accessToken, updateUI);
              profileButton.style.display = 'none';
              mailButton.style.display = 'initial';
          }
      })
      .catch(error => {
          console.log("silent token acquisition fails. acquiring token using redirect");
          // fallback to interaction when silent call fails
          return myMSALObj.acquireTokenRedirect(request)
      });
}

function seeProfile() {
  getTokenRedirect(msalConfig.loginRequest, graphConfig.graphMeEndpoint);
}

