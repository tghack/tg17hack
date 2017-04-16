# Writeup:

Her så har det blitt brukt et C++ lib, kalt [PNG++] (http://www.nongnu.org/pngpp/doc/0.2.9/), for skifte den siste biten i hver R, G og B verdi. Se kode for hvordan det er blitt gjort (Kode skrevet av Jonathan Ringstad)


Løsning også vedlagt. For å teste løsningsforslag: cmake /path/here && make. Kjør så "./stego_encode <inputfile.png> <secret.png> <output.png>" (for å skjule flag) eller "./stego_decode <input.png> <output.png>" (for å hente ut flag). Begge kjøres uten argumenter

##Flag: TG17{You_Prob_Hate_Me_Now}
