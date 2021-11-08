#include <Thermistor.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 20); 
EthernetServer server(80);     

void setup() 
{
    pinMode(5,INPUT);
    Ethernet.begin(mac, ip);  
    server.begin();           
} 

void loop() {

    EthernetClient client = server.available();  

    if (client) 
    {    
        boolean currentLineIsBlank = true;
        while (client.connected()) 
        {
            if (client.available()) 
            {   
                char c = client.read(); 
                
                if (c == '\n' && currentLineIsBlank) {
                  
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println("Refresh: 2");
                    client.println();
                    
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<title>Web serve</title>");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h1><center><font color=#FFA500>Sensor de temperatura</font></center></h1>");
                    client.println("<hr/>");
                    client.println("<h1>Temperatura</h1>");
                    porta_analogica(client);   
                    client.println("<br/>");                   
                    client.println("</body>");
                    client.println("</html>");
                    break;
                }
                
                if (c == '\n') 
                {    
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') 
                {
                    currentLineIsBlank = false;
                }
            } 
        } 
        
        delay(1);      
        client.stop(); 
        
    } 
} 

void porta_analogica(EthernetClient client_aux)
{
  int valor = analogRead(A0);
  float temperatura = (((valor*5)/1023)-0.5)*100;
  if(temperatura >=50)
  {
      client_aux.println("<font color=#8B0000>");
      client_aux.println(temperatura);
      client_aux.println("graus");
      client_aux.println("</font>");
  }
  else
  {
      client_aux.println("<font color=#000000>");
      client_aux.println(temperatura);
      client_aux.println("graus");
      client_aux.println("</font>");
  }
}


void porta_digital(EthernetClient client_aux)
{
  client_aux.print("Pino digital 5:");
  bool valor = digitalRead(5);
  if(valor == HIGH)
  {
      client_aux.println("<font color=#FF0000> Ligado</font>");
  }
  else
  {
      client_aux.println("<font color=#000000> Desligado</font>");
  }
}
