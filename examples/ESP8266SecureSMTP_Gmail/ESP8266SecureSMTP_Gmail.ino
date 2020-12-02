#include <ESP8266WiFi.h>
#include <ESP8266SecureSMTP.h>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  Serial.print("connected, address=");
  Serial.println(WiFi.localIP());
  Serial.println();

  uint32_t startTime = millis();
  
  SMTP.setEmail("YOUR_EMAIL_ADDRESS@gmail.com")
    .setPassword("YOUR_PASSWORD")
    .Subject("ESP8266SecureSMTP Gmail test")
    .setFrom("ESP8266SecureSMTP")
    .setForGmail();    // simply sets port to 465 and setServer("smtp.gmail.com");

  //SMTP.setInsecure(); // Alternatively, uncomment this and comment SMTP.setFingerprint below

   const uint8_t fingerprint[] = {0x94,0x44,0x97,0xD1,0xC5,    // smtp.gmail.com fingerprint
                                  0x65,0x8B,0x2F,0x9F,0xC5,
                                  0xE3,0xC3,0xCD,0x05,0x93,
                                  0xF2,0x2D,0x99,0x60,0xFC};
  SMTP.setFingerprint(fingerprint);

                      // message text from http://www.blindtextgenerator.com/lorem-ipsum
  if(SMTP.Send("Recipient Email, use comma for multiple recipients", "The European languages are members of the same family. Their separate existence is a myth. For science, music, sport, etc, Europe uses the same vocabulary. The languages only differ in their grammar, their pronunciation and their most common words. Everyone realizes why a new common language would be desirable: one could refuse to pay expensive translators. To achieve this, it would be necessary to have uniform grammar, pronunciation and more common words. If several languages coalesce, the grammar of the resulting language is more simple and regular than that of the individual languages. The new common language will be more simple and regular than the existing European languages. It will be as simple as Occidental; in fact, it will be Occidental. To an English person, it will seem like simplified English, as a skeptical Cambridge friend of mine told me what Occidental is. Far far away, behind the word mountains, far from the countries Vokalia and Consonantia, there live the blind texts. Separated they live in Bookmarksgrove right at the coast of the Semantics, a large language ocean. A small river named Duden flows by their place and supplies it with the necessary regelialia. It is a paradisematic country, in which roasted parts of sentences fly into your mouth. Even the all-powerful Pointing has no control about the blind texts it is an almost unorthographic life One day however a small line of blind text by the name of Lorem Ipsum decided to leave for the far World of Grammar. The Big Oxmox advised her not to do so, because there were thousands of bad Commas, wild Question Marks and devious Semikoli, but the Little Blind Text didn’t listen. She packed her seven versalia, put her initial into the belt and made herself on the way. When she reached the first hills of the Italic Mountains, she had a last view back on the skyline of her hometown Bookmarksgrove, the headline of Alphabet Village and the subline of her own road, the Line Lane. Pityful a rethoric question ran over her cheek, then she continued her way. On her way she met a copy. The copy warned the Little Blind Text, that where it came from it would have been rewritten a thousand times and everything that was left from its origin would be the word and and the Little Blind Text should turn around and return to its own, safe country. But nothing the copy said could convince her and so it didn’t take long until a few insidious Copy Writers ambushed her, made her drunk with Longe and Parole and dragged her into their agency, where they abused her for their projects again and again. And if she hasn’t been rewritten, then they are still using her. A wonderful serenity has taken possession of my entire soul, like these sweet mornings of spring which I enjoy with my whole heart. I am alone, and feel the charm of existence in this spot, which was created for the bliss of souls like mine. I am so happy, my dear friend, so absorbed in the exquisite sense of mere tranquil existence, that I neglect my talents.")) {
    Serial.println(F("Message sent"));
  } else {
    Serial.print(F("Error sending message: "));
    Serial.println(SMTP.getError());
  }
  
  Serial.println(millis() - startTime);
}

void loop()
{}
