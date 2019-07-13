#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#ifndef STASSID
#define STASSID "ACSWebServer"
#define STAPSK  "12345678"
#endif

const uint16_t TVLED = 5;
const int LUZ = 4;

const char* ssid = STASSID;
const char* password = STAPSK;

WiFiServer server(81);

IRsend irsend(TVLED);

boolean route(String url, String str){
  return url.indexOf(str) != -1;
}

void light(){
  if(digitalRead(LUZ)){
    digitalWrite(LUZ, LOW);
  }else{
    digitalWrite(LUZ, HIGH);
  }
}

void tv(String req){
  if(route(req, "c/toggle")){
    irsend.sendSAMSUNG(0xE0E040BF);
  }else if(route(req, "c/menu")){
    irsend.sendSAMSUNG(0xE0E058A7);
  }else if(route(req, "c/tools")){
    irsend.sendSAMSUNG(0xE0E0D22D);
  }else if(route(req, "c/return")){
    irsend.sendSAMSUNG(0xE0E01AE5);
  }else if(route(req, "c/apps")){
    irsend.sendSAMSUNG(0xE0E09E61);
  }else if(route(req, "c/up")){
    irsend.sendSAMSUNG(0xE0E006F9);
  }else if(route(req, "c/down")){
    irsend.sendSAMSUNG(0xE0E08679);
  }else if(route(req, "c/left")){
    irsend.sendSAMSUNG(0xE0E0A659);
  }else if(route(req, "c/right")){
    irsend.sendSAMSUNG(0xE0E046B9);
  }else if(route(req, "c/enter")){
    irsend.sendSAMSUNG(0xE0E016E9);
  }else if(route(req, "channel/down")){
    irsend.sendSAMSUNG(0xE0E008F7);
  }else if(route(req, "channel/up")){
    irsend.sendSAMSUNG(0xE0E048B7);
  }else if(route(req, "volume/down")){
    irsend.sendSAMSUNG(0xE0E0D02F);
  }else if(route(req, "volume/up")){
    irsend.sendSAMSUNG(0xE0E0E01F);
  }else if(route(req, "c/1")){
    irsend.sendSAMSUNG(0xE0E020DF);
  }else if(route(req, "c/2")){
    irsend.sendSAMSUNG(0xE0E0A05F);
  }else if(route(req, "c/3")){
    irsend.sendSAMSUNG(0xE0E0609F);
  }else if(route(req, "c/4")){
    irsend.sendSAMSUNG(0xE0E010EF);
  }else if(route(req, "c/5")){
    irsend.sendSAMSUNG(0xE0E0906F);
  }else if(route(req, "c/6")){
    irsend.sendSAMSUNG(0xE0E050AF);
  }else if(route(req, "c/7")){
    irsend.sendSAMSUNG(0xE0E030CF);
  }else if(route(req, "c/8")){
    irsend.sendSAMSUNG(0xE0E0B04F);
  }else if(route(req, "c/9")){
    irsend.sendSAMSUNG(0xE0E0708F);
  }else if(route(req, "c/0")){
    irsend.sendSAMSUNG(0xE0E08877);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LUZ, OUTPUT);
  digitalWrite(LUZ, HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();
  irsend.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  client.setTimeout(5000);
  String req = client.readStringUntil('\r');
  boolean isPost = false;
  if (route(req, "/light/toggle")) {
    light();
    isPost = true;
  }else if (route(req, "/tv/")) {
    tv(req);
    isPost = true;
  }else if(route(req, "POST")){
    isPost = true;
  }
  while (client.available()) {
    client.read();
  }
  if(isPost){
    client.print(F("HTTP/1.1 200 OK\r\n"));
    client.print(F("Content-Type: text/html\r\n"));
    client.print(F("Server: Anderson ESP8266 : 1.0\r\n"));
    client.print(F("Access-Control-Allow-Origin: *\r\n"));
    client.print(F("\r\n"));
    client.print(F("<h1>Done</h1>\r\n"));    
  }else{
    client.print(F("HTTP/1.1 200 OK\r\n"));
    client.print(F("Content-Type: text/html\r\n"));
    client.print(F("Server: Anderson ESP8266 : 1.0\r\n"));
    client.print(F("Access-Control-Allow-Origin: *\r\n"));
    client.print(F("\r\n"));
    client.print(F("<!DOCTYPE html>\r\n"));
    client.print(F("<html lang=\"pt-br\">\r\n"));
    client.print(F("<head>\r\n"));
    client.print(F("<title>Sweet Home</title>\r\n"));
    client.print(F("<meta charset=\"UTF-8\">\r\n"));
    client.print(F("<meta name=\"theme-color\" content=\"#00004d\" />\r\n"));
    client.print(F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"));
    client.print(F("<link href=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGAAAABgCAYAAADimHc4AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAtJSURBVHhe7Z17UFPZHcdPkLckQCCB8Ior7rpWtupWZBVfyBtBEXVUcKRTqnZ0rLjqjM5YdfpHO9M/7Exn3Gmd\r\n"));
    client.print(F("TnXsLoUWkIgJSnFXbbXquF0VRV5RQAXUishDMDzS3wnnRsi9QZJccgOez8yZnHOC1+T7vef3+x1yb0AUCoVCoVAoHyQi8jhhGBwcFCUmJs6vrq7e1NPTM0Ov1yMPD48GpVKZp9Fobnh7e+vJj1L45siRIx6hoaF/cHJy6oEhFtrYRCLRW4VC8Zd9+/aJYUzhm9zc3Kn+\r\n"));
    client.print(F("/v5noTsIbYT4w5uvr+93W7Zs8YE+hS927949VSaTaaDLKbppk0ql/8rOzqYm8MH+/fvFIGgpdFlCO4mGGtdzsBKubNu2TQp9irUcOHBAAkJyiv+pP9J/uRDp9y1C+jkB7Odxk0gk327fvt0f+hRLOXjwoLePjw+n+LNlSP+rpUh/dPlQO7wM6T9XsH8ON7FY/B2YIIO+\r\n"));
    client.print(F("QzKFPDoUe/fu9T5x4sQ37e3tqWTKyGw5Qhmz4IU7kQlABMX0x34IdeoQaukikwSdTjetrq4uauPGjefu3Lnzhkw7DA5nwK5duySnT58uePXqVQqZMhLBIT4DNuETMKGrD0zoJJOEt2/fKhsaGqLXrVunqqysxCWsw+BQBmRmZnqVlJQUt7W1JZApI5+B+Guw+KNsHQ0m\r\n"));
    client.print(F("QNrtBhOa2SaENTU1LU1PTy+5f/++w5jgMAZs3bpVXFpaegbEjyNTRn4cgFD6e8RnMIQjMOFNP6cJIU+fPl2empp6pqqqyiFMcAgDdu7c6V1QUFAIYSeeTBmZEwjifzo28RkYE3rAhKcmJvT29gZjE0g4EjwnCG4AbLJ8Ieb/HRIuS/y5IP7qmQhBrW8x2IQZYEIvhwmw\r\n"));
    client.print(F("EoIbGxuXbdq0qfT27dvdZFoQBDVgz5490pMnTxa8fv2aU/xVVorPYDRhAEzoIJMEbIJWq10CeUcNJpjUTvZDMAMg7PidOnUqv6OjgyX+PB7EZzCY4AuCgwlPTEyAEjWkvr5+cVZWVhmYYLJO7IMgBkD89VapVEVw5seSKSPzFAil8SQ+AzYhHFbCWwhHXCbglQCvSQ05\r\n"));
    client.print(F("we4m2N2A1atXe1+6dEkFMX85mTICu1mU9gm/4jPgQ2ITdNwm4JwQk5ycXFJdXW3XnGBXAyDp+Vy8eLEUqp2lZMrIT4IQSh0n8RkYE/ogHD1m5wRFc3NzPHCmtrbWbibYzYDs7Gwp1PklcOazxJ8P4q/8eHzFZ8D/xXQwoX8QTHg9NMcAJgS2trbGr1y5UvXgwQO7JGa7\r\n"));
    client.print(F("GACbLP/CwsJiSLgs8SPtKD6DwQRIzANgQhOHCY8fP45bs2ZNKeyYxz0njLsBO3bskOXl5RV1dnayxF8QjFAKiI+TpL3B/+VHYMIghwmQEwIbGhpWQGLW3Lt3zyRY8cu4GrB582Z/2OEWdXV1scSPAvGTZwgjPgOzEgb1CDWamNDX16d4+PDh8vXr15+H6sjkWf4YNwMy\r\n"));
    client.print(F("MjL8NBqNCsRfQqaMYPGTBDrzuTCsBPMmxKSnp2uqqqrGxYRxMQCSmPTy5ctqiPmLyJSRL0KI+GTsKGAT9NwmBDY1NcWlpqaeg8TMezji3YCUlBS/q1evamCT9QWZMrIQxE/EYYeMHQ1sAqahfeiRAXJCACTmxBUrVqhg58xrYubVgLVr18pgk3UOzvwoMmVkUShCCeGO\r\n"));
    client.print(F("E3bMMc1n6AThMEEGJWpSbGzsWTCBt5XAmwGZmZkBZWVlKqh2WGd+NIgfPwHEZ8Am4LL4EYcJLS0tSbBjVtfU1PCSE3gxAKqdwOLi4qLu7m5WzMfix2HxyXiioAQT8GcQpiZATpBBOIpPS0srg5xg8qzl2GwAlGkBZ8+ePdPT08MSf3HYxBSfYRQT5LBPiIXqqByqo1dk\r\n"));
    client.print(F("2ipsMmDVqlXy8vLyc2/evGHF/CUgfuz0iSs+AzbB2QlMMJG5v79f/ujRI1wdXaiurrbaBKsNSExMlF+5ckUDYWc+mTKyRDk5xGcI80bIhdsEGZSoCaBFWW1trVUmWGVATEyM/MaNG+dhk/U5mTKyFIv/ERlMIgwmgFqmJkA48n/y5ElKQkKCuq6uzmITLDYgKSkp8Pr1\r\n"));
    client.print(F("61j8eWTKyDIQf8UkFJ8Bm+AGij1km+DX3NycCiXqOShRLTLBIgMg5gdDna+GsMMSf/k0WBmTWHyGUDDB3RkhbRuZIIAJUihR08AEtSUmjNkA2GSFXrhwQQUJlxV2YkB8bMCHQojErAm+EI6SICeUQzh6SaZHZUwGgPhharW6pLe3lyU+DjnLPiDxGbAJHhwmQGL2w4k5\r\n"));
    client.print(F("JSWlAhLz/8i0Wd5rAISdMHzmg/issIPFx0n3QwWb4OmCkGnAGRgY8G9sbIS8nFAB4WhUE0atFKOiogIrKyvLIex8RqYM4H+ExcflJgWhm08RKqsf+m3qcNzd3bURERGxt27daiRTLDiuMx4iPz/fDbbaf+MSH9f4VPx3GD7Z4/hwCaJGOOyUiw8dOmT2xkGzIQiWTgos\r\n"));
    client.print(F("owPQNR4Wd+JAfPwrBspIgiEcebkiVMdOzAqojhra2tq+J1MjMGuAXq/fCeWm8TebWPx4ED+aim+WIDjPxWBCPZgwPBrh22ohkhSS4QhMFs07goKC/gGbi3VkaNiA4KTzPtb+aCgxTUbwpSz599ix3pTmLoR6+sgACAgIqHj27BnrEkyM2RwACeQB6RrA11ZqIdu/r+EX\r\n"));
    client.print(F("OVnBwnO9Z9M2XHyMq6trNemyMGtAZGRk4ZQpUxzqdp6JiEgk0s2aNSufDFmYNaCgoOBuaGjor6EL5z7FSgYVCsXvy8vLr5IxC7MGYCoqKn43Z86cn0kkkv/CMnrt7OzcObzBCnG4uw7tDdbAVBfQqkMsFt+dPXv2L48fP36I/CgnZpPwcFpbW6dcu3bNp729fYRhRUVF\r\n"));
    client.print(F("s9Rq9bfQNVZTXy5ESOJGBpMMfFHvb/49Ignro6Oj03Jycm6SsQE4YQchhLcrlcr3Ro8xGWCOuXPnRtwGoPvBGhAeHr5Yq9VeI2OLGTUEUcYfaoDAOKQBz7sRamznt7UKdhve6DhkDvj67tB2nk/wLv7nrE8zLIPmgEkINUBgqAECMyFygFQq/cHHx0dNhmOis7Mz9sWL\r\n"));
    client.print(F("F/BqhnDUHDAhDJDL5X96/vz5L8hwTISFhf22qakJf6BkgCZhCifUAIGhBggMNUBgqAECQw0QGGqAwFADBIYaIDDUAIGhBggMNUBgqAECQw0QGGqAwFADBIYaIDDUAIGhBgiMTQaIJspXYDkwNhmgVCrffTxN6NKRjg2YHsPFxcXi72jD1+iTrgF8TPzVlLbQAccY9oG8\r\n"));
    client.print(F("gYCAAJuOapMBycnJzSDOiKsuKx4O3U9mLd83s6/jlEgk9aQ7Zkzvy2rvReg/j9kCjpW+QXhvWjIg4D8gumDBAjiqQOA/LSuTyf4JXfy2jM3TBemDxEgfbGHz9xx5HNzwLZ5ZWVkzoG8Rubm5Cjg58BfrjTie1IP7/x6t4ffi5TryOIZjSaU3a2pqjJfkCEJkZGQyPOD7\r\n"));
    client.print(F("AlkvkI8GS/yblpYWq5JNUFDQcXjgPC4PbSAiImIjPAqLVqt1UigUX0EX36DK9UKtbu7u7vUbNmwIhr5V5OTkyD09PSuhy3l8W5pcLv/r+fPnhT37GY4ePeoaEhLyR4iJvK0ELy+vewkJCRHQt4mMjIzpYrEY38PF1wnSHxgY+DX++8bQtxne6siOjg4nECwOVkSOTqeL\r\n"));
    client.print(F("hCbGf27cEpydnXWQPLV+fn6qtLS0Px87doyXL0c9fPjw1IKCgp++fPlyQ29v7/T+/n6LLp7E5TYuNtzc3H4ICws7lZeXp5k5c2Y/edomxqWQhzeKv6zA4itE4Y0OQGIb15vD29ra3OHEcCZDS9DBicFDkU2hUCgUCsUBQOj/Bkh77X51PmAAAAAASUVORK5CYII=\"\r\n"));
    client.print(F("rel=\"icon\" type=\"image/x-icon\" />\r\n"));
    client.print(F("<script>\r\n"));
    client.print(F("function p(u){var s=document.getElementById(\"spinner\");if(s.hidden){s.hidden=false;var x=new XMLHttpRequest();x.onreadystatechange=function(){if(this.readyState===4){if(this.status!==200)alert(\"Error on Connection.\\nCode: \"+this.status);s.hidden=true;}};x.open(\"POST\",u,true);x.send();}}\r\n"));
    client.print(F("</script>\r\n"));
    client.print(F("<style>\r\n"));
    client.print(F("body {background-color: #00004d; text-align: center; }\r\n"));
    client.print(F("svg {margin: auto; height: 72px; width: 72px;}\r\n"));
    client.print(F("input { background-color: #00004d; color: green; font-size: 1.3em; text-align: center; border-color: red;}\r\n"));
    client.print(F(".down {transform: rotate(90deg);}\r\n"));
    client.print(F(".up {transform: rotate(270deg);}\r\n"));
    client.print(F(".left {transform: rotate(180deg);}\r\n"));
    client.print(F(".right {transform: rotate(0deg);}\r\n"));
    client.print(F("table {margin: auto;}\r\n"));
    client.print(F("table .num {align-content: center; background-image: \r\n"));
    client.print(F("url(\"data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij4NCjxwYXRoIGZpbGw9Im5vbmUiIGQ9Ik0wIDBoMjR2MjRIMFYweiIvPg0KPHBhdGggZmlsbD0iZ3JlZW4iIGQ9Ik0xNyAzSDdjLTEuMSAwLTIgLjktMiAydjE0YzAgMS4xLjkgMiAyIDJoMTBjMS4xIDAgMi0uOSAyLTJWNWMwLTEuMS0uOS0yLTItMnptLTEgMTZIOGMtLjU1IDAtMS0uNDUtMS0xVjZjMC0uNTUuNDUtMSAxLTFoOGMuNTUgMCAxIC40NSAxIDF2MTJjMCAuNTUtLjQ1IDEtMSAxeiIvPg0KPC9zdmc+\"); \r\n"));
    client.print(F("background-repeat: no-repeat; background-size: cover; height: 80px; width: 80px}\r\n"));
    client.print(F("span {margin: auto; font-size: 2.7em; color: green; font-weight: 700}\r\n"));
    client.print(F("#spinner {position: fixed; top: 30%; left: 50%;}\r\n"));
    client.print(F("#spinner-svg {position: absolute; width: 168px; height: 168px; top: -86px; left: -86px; animation: App-logo-spin infinite 1s linear; height: 40vmin; pointer-events: none; }\r\n"));
    client.print(F("@keyframes App-logo-spin { from { transform: rotate(0deg); } to { transform: rotate(360deg); } }\r\n"));
    client.print(F("</style>\r\n"));
    client.print(F("</head>\r\n"));
    client.print(F("<body>\r\n"));
    client.print(F("<br><br>\r\n"));
    client.print(F("<table>\r\n"));
    client.print(F("<thead>\r\n"));
    client.print(F("<tr>\r\n"));
    client.print(F("<th colspan=\"6\" >\r\n"));
    client.print(F("<span>Sweet Home</span>\r\n"));
    client.print(F("</th>\r\n"));
    client.print(F("</tr>\r\n"));
    client.print(F("</thead>\r\n"));
    client.print(F("<tbody>\r\n"));
    client.print(F("<tr>\r\n"));
    client.print(F("<td colspan=\"3\" onclick=\"p('/light/toggle')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 24 24\"><path fill=\"none\" d=\"M0 0h24v24H0V0z\"/><path fill=\"yellow\" stroke=\"black\" d=\"M4.25 19.79c.39.39 1.02.39 1.41 0l.39-.39c.39-.39.38-1.02 0-1.4l-.01-.01c-.39-.39-1.02-.39-1.41 0l-.39.39c-.38.4-.38 1.02.01 1.41zM11.99 23H12c.55 0 .99-.44.99-.99v-.96c0-.55-.44-.99-.99-.99h-.01c-.55 0-.99.44-.99.99v.96c0 .55.44.99.99.99zM3.01 11.05H1.99c-.55 0-.99.44-.99.99v.01c0 .55.44.99.99.99H3c.55 0 .99-.44.99-.99v-.01c.01-.55-.43-.99-.98-.99zM15 6.86V3.05c0-.55-.45-1-1-1h-4c-.55 0-1 .45-1 1v3.81c-2.04 1.18-3.32 3.52-2.93 6.13.4 2.61 2.56 4.7 5.18 5.02 3.64.44 6.75-2.4 6.75-5.95 0-2.23-1.21-4.16-3-5.2zm5 5.18v.01c0 .55.44.99.99.99H22c.55 0 .99-.44.99-.99v-.01c0-.55-.44-.99-.99-.99h-1.01c-.55 0-.99.44-.99.99zm-2.06 7.37l.39.39c.39.39 1.02.39 1.41 0 .39-.39.39-1.02 0-1.41l-.39-.39c-.39-.39-1.02-.38-1.4 0-.4.4-.4 1.02-.01 1.41z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("<td colspan=\"3\" onclick=\"p('/tv/c/toggle')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 24 24\"><path fill=\"none\" d=\"M0 0h24v24H0V0z\"/><path fill=\"red\" stroke=\"black\" d=\"M12 3c-.55 0-1 .45-1 1v8c0 .55.45 1 1 1s1-.45 1-1V4c0-.55-.45-1-1-1zm5.14 2.86c-.39.39-.38 1-.01 1.39 1.13 1.2 1.83 2.8 1.87 4.57.09 3.83-3.08 7.13-6.91 7.17C8.18 19.05 5 15.9 5 12c0-1.84.71-3.51 1.87-4.76.37-.39.37-1-.01-1.38-.4-.4-1.05-.39-1.43.02C3.98 7.42 3.07 9.47 3 11.74c-.14 4.88 3.83 9.1 8.71 9.25 5.1.16 9.29-3.93 9.29-9 0-2.37-.92-4.51-2.42-6.11-.38-.41-1.04-.42-1.44-.02z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("</tr>\r\n"));
    client.print(F("<tr>\r\n"));
    client.print(F("<td colspan=\"3\" onclick=\"p('/tv/channel/down')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" class=\"down\" viewBox=\"0 0 24 24\"><path fill=\"none\" d=\"M0 0h24v24H0V0z\"/><path fill=\"blue\" stroke=\"black\" d=\"M3.4 20.4l17.45-7.48c.81-.35.81-1.49 0-1.84L3.4 3.6c-.66-.29-1.39.2-1.39.91L2 9.12c0 .5.37.93.87.99L17 12 2.87 13.88c-.5.07-.87.5-.87 1l.01 4.61c0 .71.73 1.2 1.39.91z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("<td colspan=\"3\" onclick=\"p('/tv/channel/up')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" class=\"up\" viewBox=\"0 0 24 24\"><path fill=\"none\" d=\"M0 0h24v24H0V0z\"/><path fill=\"blue\" stroke=\"black\" d=\"M3.4 20.4l17.45-7.48c.81-.35.81-1.49 0-1.84L3.4 3.6c-.66-.29-1.39.2-1.39.91L2 9.12c0 .5.37.93.87.99L17 12 2.87 13.88c-.5.07-.87.5-.87 1l.01 4.61c0 .71.73 1.2 1.39.91z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("</tr>\r\n"));
    client.print(F("<tr>\r\n"));
    client.print(F("<td colspan=\"3\" onclick=\"p('/tv/volume/down')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"24\" height=\"24\" viewBox=\"0 0 24 24\"><path fill=\"blue\" stroke=\"black\" d=\"M18.5 12c0-1.77-1.02-3.29-2.5-4.03v8.05c1.48-.73 2.5-2.25 2.5-4.02zM5 10v4c0 .55.45 1 1 1h3l3.29 3.29c.63.63 1.71.18 1.71-.71V6.41c0-.89-1.08-1.34-1.71-.71L9 9H6c-.55 0-1 .45-1 1z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("<td colspan=\"3\" onclick=\"p('/tv/volume/up')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 24 24\"><path fill=\"blue\" stroke=\"black\" d=\"M3 10v4c0 .55.45 1 1 1h3l3.29 3.29c.63.63 1.71.18 1.71-.71V6.41c0-.89-1.08-1.34-1.71-.71L7 9H4c-.55 0-1 .45-1 1zm13.5 2c0-1.77-1.02-3.29-2.5-4.03v8.05c1.48-.73 2.5-2.25 2.5-4.02zM14 4.45v.2c0 .38.25.71.6.85C17.18 6.53 19 9.06 19 12s-1.82 5.47-4.4 6.5c-.36.14-.6.47-.6.85v.2c0 .63.63 1.07 1.21.85C18.6 19.11 21 15.84 21 12s-2.4-7.11-5.79-8.4c-.58-.23-1.21.22-1.21.85z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("</tr>\r\n"));
    client.print(F("<tr>\r\n"));
    client.print(F("<td colspan=\"2\" onclick=\"p('/tv/c/tools')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 24 24\"><path fill=\"none\" d=\"M0 0h24v24H0V0z\"/><path fill=\"#800040\" stroke=\"black\" d=\"M12 8c1.1 0 2-.9 2-2s-.9-2-2-2-2 .9-2 2 .9 2 2 2zm0 2c-1.1 0-2 .9-2 2s.9 2 2 2 2-.9 2-2-.9-2-2-2zm0 6c-1.1 0-2 .9-2 2s.9 2 2 2 2-.9 2-2-.9-2-2-2z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("<td colspan=\"2\" onclick=\"p('/tv/c/up')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" class=\"up\" viewBox=\"0 0 24 24\"><path fill=\"#800040\" stroke=\"black\" d=\"M8 6.82v10.36c0 .79.87 1.27 1.54.84l8.14-5.18c.62-.39.62-1.29 0-1.69L9.54 5.98C8.87 5.55 8 6.03 8 6.82z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("<td colspan=\"2\" onclick=\"p('/tv/c/apps')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"24\" height=\"24\" viewBox=\"0 0 24 24\"><path fill=\"none\" d=\"M0 0h24v24H0V0z\"/><path fill=\"#800040\" stroke=\"black\" d=\"M4 8h4V4H4v4zm6 12h4v-4h-4v4zm-6 0h4v-4H4v4zm0-6h4v-4H4v4zm6 0h4v-4h-4v4zm6-10v4h4V4h-4zm-6 4h4V4h-4v4zm6 6h4v-4h-4v4zm0 6h4v-4h-4v4z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("</tr>\r\n"));
    client.print(F("<tr>\r\n"));
    client.print(F("<td colspan=\"2\" onclick=\"p('/tv/c/left')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" class=\"left\" viewBox=\"0 0 24 24\"><path fill=\"#800040\" stroke=\"black\" d=\"M8 6.82v10.36c0 .79.87 1.27 1.54.84l8.14-5.18c.62-.39.62-1.29 0-1.69L9.54 5.98C8.87 5.55 8 6.03 8 6.82z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("<td colspan=\"2\" onclick=\"p('/tv/c/enter')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 24 24\"><g fill=\"none\"><path d=\"M0 0h24v24H0V0z\"/><path opacity=\".87\" d=\"M0 0h24v24H0V0z\"/></g><path fill=\"#800040\" stroke=\"black\" d=\"M21 3.01H3c-1.1 0-2 .9-2 2V8c0 .55.45 1 1 1s1-.45 1-1V5.99c0-.55.45-1 1-1h16c.55 0 1 .45 1 1v12.03c0 .55-.45 1-1 1H4c-.55 0-1-.45-1-1V16c0-.55-.45-1-1-1s-1 .45-1 1v3.01c0 1.09.89 1.98 1.98 1.98H21c1.1 0 2-.9 2-2V5.01c0-1.1-.9-2-2-2zm-9.15 12.14l2.79-2.79c.2-.2.2-.51 0-.71l-2.79-2.79c-.31-.32-.85-.1-.85.35V11H2c-.55 0-1 .45-1 1s.45 1 1 1h9v1.79c0 .45.54.67.85.36z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("<td colspan=\"2\" onclick=\"p('/tv/c/right')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" class=\"right\" viewBox=\"0 0 24 24\"><path fill=\"#800040\" stroke=\"black\" d=\"M8 6.82v10.36c0 .79.87 1.27 1.54.84l8.14-5.18c.62-.39.62-1.29 0-1.69L9.54 5.98C8.87 5.55 8 6.03 8 6.82z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("</tr>\r\n"));
    client.print(F("<tr>\r\n"));
    client.print(F("<td colspan=\"2\" onclick=\"p('/tv/c/menu')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 24 24\"><path fill=\"none\" d=\"M0 0h24v24H0V0z\"/><path fill=\"#800040\" stroke=\"black\" d=\"M19.43 12.98c.04-.32.07-.64.07-.98s-.03-.66-.07-.98l2.11-1.65c.19-.15.24-.42.12-.64l-2-3.46c-.12-.22-.39-.3-.61-.22l-2.49 1c-.52-.4-1.08-.73-1.69-.98l-.38-2.65C14.46 2.18 14.25 2 14 2h-4c-.25 0-.46.18-.49.42l-.38 2.65c-.61.25-1.17.59-1.69.98l-2.49-1c-.23-.09-.49 0-.61.22l-2 3.46c-.13.22-.07.49.12.64l2.11 1.65c-.04.32-.07.65-.07.98s.03.66.07.98l-2.11 1.65c-.19.15-.24.42-.12.64l2 3.46c.12.22.39.3.61.22l2.49-1c.52.4 1.08.73 1.69.98l.38 2.65c.03.24.24.42.49.42h4c.25 0 .46-.18.49-.42l.38-2.65c.61-.25 1.17-.59 1.69-.98l2.49 1c.23.09.49 0 .61-.22l2-3.46c.12-.22.07-.49-.12-.64l-2.11-1.65zM12 15.5c-1.93 0-3.5-1.57-3.5-3.5s1.57-3.5 3.5-3.5 3.5 1.57 3.5 3.5-1.57 3.5-3.5 3.5z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("<td colspan=\"2\" class=\"down\" onclick=\"p('/tv/c/down')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 24 24\"><path fill=\"#800040\" stroke=\"black\" d=\"M8 6.82v10.36c0 .79.87 1.27 1.54.84l8.14-5.18c.62-.39.62-1.29 0-1.69L9.54 5.98C8.87 5.55 8 6.03 8 6.82z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("<td colspan=\"2\" onclick=\"p('/tv/c/return')\">\r\n"));
    client.print(F("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 24 24\"><path fill=\"none\" d=\"M0 0h24v24H0V0z\"/><path fill=\"#800040\" stroke=\"black\" d=\"M12.5 8c-2.65 0-5.05.99-6.9 2.6L3.71 8.71C3.08 8.08 2 8.52 2 9.41V15c0 .55.45 1 1 1h5.59c.89 0 1.34-1.08.71-1.71l-1.91-1.91c1.39-1.16 3.16-1.88 5.12-1.88 3.16 0 5.89 1.84 7.19 4.5.27.56.91.84 1.5.64.71-.23 1.07-1.04.75-1.72C20.23 10.42 16.65 8 12.5 8z\"/></svg>\r\n"));
    client.print(F("</td>\r\n"));
    client.print(F("</tr>\r\n"));
    client.print(F("<tr>\r\n"));
    client.print(F("<td colspan=\"2\" class=\"num\" onclick=\"p('/tv/c/1')\"><span>1</span></td><td colspan=\"2\" class=\"num\" onclick=\"p('/tv/c/2')\"><span>2</span></td><td colspan=\"2\" class=\"num\" onclick=\"p('/tv/c/3')\"><span>3</span></td>\r\n"));
    client.print(F("</tr>\r\n"));
    client.print(F("<tr>\r\n"));
    client.print(F("<td colspan=\"2\" class=\"num\" onclick=\"p('/tv/c/4')\"><span>4</span></td><td colspan=\"2\" class=\"num\" onclick=\"p('/tv/c/5')\"><span>5</span></td><td colspan=\"2\" class=\"num\" onclick=\"p('/tv/c/6')\"><span>6</span></td>\r\n"));
    client.print(F("</tr>\r\n"));
    client.print(F("<tr>\r\n"));
    client.print(F("<td colspan=\"2\" class=\"num\" onclick=\"p('/tv/c/7')\"><span>7</span></td><td colspan=\"2\" class=\"num\" onclick=\"p('/tv/c/8')\"><span>8</span></td><td colspan=\"2\" class=\"num\" onclick=\"p('/tv/c/9')\"><span>9</span></td>\r\n"));
    client.print(F("</tr>\r\n"));
    client.print(F("<tr>\r\n"));
    client.print(F("<td colspan=\"2\"></td><td colspan=\"2\" class=\"num\" onclick=\"p('/tv/c/0')\"><span>0</span></td><td colspan=\"2\"></td>\r\n"));
    client.print(F("</tr>\r\n"));
    client.print(F("</tbody>\r\n"));
    client.print(F("</table>\r\n"));
    client.print(F("<br><br>\r\n"));
    client.print(F("<div id=\"spinner\" hidden >\r\n"));
    client.print(F("<svg id=\"spinner-svg\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 24 24\"><path fill=\"none\" d=\"M0 0h24v24H0V0z\"/><path fill=\"#804000\" stroke=\"black\" d=\"M10 19v-5h4v5c0 .55.45 1 1 1h3c.55 0 1-.45 1-1v-7h1.7c.46 0 .68-.57.33-.87L12.67 3.6c-.38-.34-.96-.34-1.34 0l-8.36 7.53c-.34.3-.13.87.33.87H5v7c0 .55.45 1 1 1h3c.55 0 1-.45 1-1z\"/></svg>\r\n"));
    client.print(F("</div>\r\n"));
    client.print(F("</body>\r\n"));
    client.print(F("</html>\r\n"));
  }
}
