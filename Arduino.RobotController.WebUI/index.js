// Select DOM elements to work with
const userDetailsDiv = document.getElementById("userDetailsDiv");
const unauthenticatedActionsDiv = document.getElementById("unauthenticatedActionsDiv");
const authenticatedActionsDiv = document.getElementById("authenticatedActionsDiv");
const signInButton = document.getElementById("signIn");
const signOutButton = document.getElementById("signOut");

const redirectUri = window.location.href.includes("localhost") ? "http://localhost:5500" : "https://ghostrobotcontroller.rodolfograve.com/"

// Config object to be passed to Msal on creation.
// For a full list of msal.js configuration parameters, 
// visit https://azuread.github.io/microsoft-authentication-library-for-js/docs/msal/modules/_authenticationparameters_.html
const msalConfig = {
  auth: {
    clientId: "78271001-f0f7-498a-a48a-7772e9264c7e",
    authority: "https://login.microsoftonline.com/a599a1f5-fa4f-4265-9ea5-fbe3bc64501d",
    redirectUri: redirectUri,
  },
  cache: {
    cacheLocation: "sessionStorage", // This configures where your cache will be stored
    storeAuthStateInCookie: false, // Set this to "true" if you are having issues on IE11 or Edge
  },
  loginRequest: {
    scopes: ["openid", "profile", "User.Read"]
  }
};  
  
const graphConfig = {
  graphMeEndpoint: "https://graph.microsoft.com/v1.0/me",
  accessTokenRequest: {
    scopes: ["Mail.Read"]
  }
};

const storageQueueConfig = {
  putMessageEndpoint: "https://ghostrobotcontroller.queue.core.windows.net/myqueue/messages",
  accessTokenRequest: {
    scopes: ["https://storage.azure.com/user_impersonation"]
  }
}

// Helper function to call MS Graph API endpoint 
// using authorization bearer token scheme
async function callMSGraph(endpoint, token) {
  const headers = new Headers();
  const bearer = `Bearer ${token}`;

  headers.append("Authorization", bearer);

  const options = {
      method: "GET",
      headers: headers
  };

  console.log('Request made to Graph API at: ' + new Date().toString());
  let response = await fetch(endpoint, options);
  let responseData = await response.json();
  console.log("Graph response was", responseData);
}

/* Details for the PUT operation can be found here: https://docs.microsoft.com/en-us/rest/api/storageservices/put-message */
async function putMessage(message) {
  let tokenResponse = await myMSALObj.acquireTokenSilent(storageQueueConfig.accessTokenRequest);
  console.debug('StorageQueue access_token acquired at: ' + new Date().toString());
  const accessToken = tokenResponse.accessToken;
  const url = "https://ghostrobotcontroller.queue.core.windows.net/commands/messages";
  const headers = new Headers();
  const bearer = `Bearer ${accessToken}`;
  const version = "2019-07-07";
  const contentType = "application/json";
  const body = `<QueueMessage><MessageText>${btoa(message)}</MessageText></QueueMessage>`;

  headers.append("Authorization", bearer);
  headers.append("x-ms-version", version);
  headers.append("Content-Type", contentType);

  const options = {
      method: "POST",
      headers: headers,
      body: body
  };

  console.debug("Sending request with body", body);
  let response = await fetch (url, options);
  let responseData = await response.text();
  console.debug("Message was put in the queue", responseData);
}

function onLoginSuccess(account) {
  hide(unauthenticatedActionsDiv);
  show(authenticatedActionsDiv);
  userDetailsDiv.innerHTML = `Welcome ${account.name}`;
  hide(signInButton);
  show(signOutButton);
}

function show(element) {
  element.classList.remove('d-none');
}

function hide(element) {
  element.classList.add('d-none');
}

function signOut() {
  myMSALObj.logout();
}