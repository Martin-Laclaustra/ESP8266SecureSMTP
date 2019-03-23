/* 
*  ESP8266SMTPHelper class helps send e-mails
*  using Arduino core for ESP8266 WiFi chip
*  by Boris Shobat
*  May 11 2017
*  Updated for BearSSL by Martin Laclaustra
*  2019-03-23
*/

#ifndef _ESP8266SecureSMTPHelper_H
#define _ESP8266SecureSMTPHelper_H

#include <WiFiClientSecure.h>

//#define GS_SERIAL_LOG_LEVEL_1      // Print to Serial only server response
//#define GS_SERIAL_LOG_LEVEL_2      // Print to Serial client commands and server response

const char HTML_HEAD[] PROGMEM = "Mime-Version: 1.0\r\nContent-Type: text/html; charset=\"UTF-8\";\r\nContent-Transfer-Encoding: 7bit;\r\n\r\n<!DOCTYPE html><html lang=\"en\">";
const char HTML_END[] PROGMEM  = "</html>\r\n.";
const char SMTP_HELO[] PROGMEM = "HELO friend";
const char SMTP_AUTH[] PROGMEM = "AUTH LOGIN";
const char SMTP_FROM[] PROGMEM = "MAIL FROM:<$>";
const char SMTP_RCPT[] PROGMEM = "RCPT TO:<$>";
const char SMTP_SUB[] PROGMEM  = "Subject: ";

class ESP8266SMTPHelper
{
  private:
  uint16_t _smtp_port = 465;
  const char *_error = nullptr;
  char
    *_smtp_server     = nullptr,
    *_emailAddress    = nullptr,
    *_base64_login    = nullptr,
    *_base64_password = nullptr,
    *_from            = nullptr,
    *_subject         = nullptr;
  String _serverResponse;
  bool AwaitSMTPResponse(WiFiClientSecure &client, const String &resp = "", uint16_t timeOut = 10000);

  // settings to pass to secure client
  bool _use_insecure;
  bool _use_fingerprint;
  uint8_t _fingerprint[20];
  bool _use_self_signed;
  const PublicKey *_knownkey;
  unsigned _knownkey_usages;
  const X509List *_ta;
  CertStore *_certStore;
  
  public:
  ESP8266SMTPHelper() = default;
  ESP8266SMTPHelper(const char*, const char*);
  ~ESP8266SMTPHelper();
  ESP8266SMTPHelper
    &setPort(uint16_t),
    &setServer(const char*),
    &Subject(const char*),
    &setFrom(const char*),
    &setEmail(const char*),
    &setPassword(const char*),
    &setForGmail();
  char* getBase64Email();
  char* getBase64Password();

  // methods for settings to pass to secure client
  void _clearAuthenticationSettings();
  // Don't validate the chain, just accept whatever is given.  VERY INSECURE!
  void setInsecure() {
    _clearAuthenticationSettings();
    _use_insecure = true;
  }
  // Assume a given public key, don't validate or use cert info at all
  void setKnownKey(const PublicKey *pk, unsigned usages = BR_KEYTYPE_KEYX | BR_KEYTYPE_SIGN) {
    _clearAuthenticationSettings();
    _knownkey = pk;
    _knownkey_usages = usages;
  }
  // Only check SHA1 fingerprint of certificate
  bool setFingerprint(const uint8_t fingerprint[20]) {
    _clearAuthenticationSettings();
    _use_fingerprint = true;
    memcpy_P(_fingerprint, fingerprint, 20);
    return true;
  }
  // only accept fingerprint as array, not as hex string (the client does)
  //bool setFingerprint(const char *fpStr);
  // Accept any certificate that's self-signed
  void allowSelfSignedCerts() {
    _clearAuthenticationSettings();
    _use_self_signed = true;
  }
  // Install certificates of trusted CAs or specific site
  void setTrustAnchors(const X509List *ta) {
    _clearAuthenticationSettings();
    _ta = ta;
  }
  // Attach a preconfigured certificate store
  void setCertStore(CertStore *certStore) {
    _certStore = certStore;
  }

  String getLastResponse();
  const char* getError();

  bool Send(const String &to, const String &message);
};

extern ESP8266SMTPHelper SMTP;
#endif
