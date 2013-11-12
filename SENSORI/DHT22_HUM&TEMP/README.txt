serve una resistenza da 4k7 tra SOURCE e V+ (vedi schema)

Questo sensore utilizza una comunicazione di tipo digitale seriale, quindi vengono meno alcuni problemi come:

La resistenza del cavo di connessione, che può inficiare la misurazione
La complessità di connessione, dato che necessita solo di tre cavi (Vcc,Data,Gnd)
Vediamone alcune caratteristiche:

Tens.Alimentazione : 3.3-6V
Assorbimento in Misura : 1-1.5mA
Assorbimento in StandBy : 40-50 uA
Umidità : 0 / 100% Relativa
Temperatura : -40 / +80 °C
Precisione Umidità : 0.1%
Precisione Temperatura 0.1°C
Errore Umidità : +-2% Relativa
Errore Temperatura : +-0.5 °C
Per la logica di trasmissione delle informazioni di questo sensore, vi rimando al datasheet: Datasheet DHT22

L'unico neo del sensore è la sua lentezza, infatti si parla di circa 2 s tra una lettura e la successiva.