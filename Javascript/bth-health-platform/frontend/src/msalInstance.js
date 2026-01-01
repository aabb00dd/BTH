// src/msalInstance.js
import { PublicClientApplication } from "@azure/msal-browser";

const msalInstance = new PublicClientApplication({
  auth: {
    clientId: '185a72d2-cb55-4dec-ad0f-76271e3d0fd7',
    authority: "https://login.microsoftonline.com/common/",
    redirectUri: "http://localhost:3000/home"
  }
});


export default msalInstance;