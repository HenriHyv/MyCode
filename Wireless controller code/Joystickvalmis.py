
import pydirectinput    #kirjasto,jolla Python simuloi tietyn painetun näppäimistön/hiiren painallusta
import serial           #kirjasto mahdollistaa datan kulkemisen tietokoneen ja arduinon välillä USB portista

arduino = serial.Serial ('COM3', 9600, timeout=.1)     #Aloitetaan lukemaan serial dataa portista COM, johon arduino on kytketty, kutsuu kerran funktiota

pydirectinput.PAUSE = 0         #voidaan laittaa delayta key press tai key releause komentoon halutessaan. Asetettu 0 eli ei delayta.

keysDown = {}                   #ottaa vastaan "key" inputtina ja listaa sen sen tällä hetkellä painettuihin näppäimiin


def keyDown(key):               #vastaanottaa painetun keyDown nappaimen
    keysDown[key] = True        #lisää painetun näppäimen KeysDown listaan
    pydirectinput.keyDown(key)  #suorittaa lopuksi pydirectinput kirjaston komennon nappaimelle



def keyUp(key):
    if key in keysDown:             #tarkistaa onko releaset nappi keysDown listassa
        del (keysDown[key])         #poistaa näppäimen keysdown listasta
        pydirectinput.keyUp(key)    #ajaa pydirectinput KeyUp komennon, että painettu nappain ei ole enää painettuna

def Nappaimet(x,y,b, c, v,p,n,n2,n3,n4,n5,n6,n7,n8):            #alhaalla määritelyjen nappaimet osion arvot siirtyy tähän aliohjelmaan
    if y == 0:              #0 = kamera ylös
        keyDown('up')                  #lisää komennon ylös keyDown argumenttiin
        keyUp('down')               #lisää komennon ylös keyDown argumenttiin, joka käy tarkistamassa että onko näppäin 'down' keydownissa ja poistaa sen


    elif y == 2:         #2 = kamera alas
        keyDown('down')
        keyUp('up')
    else:               #1 = ei toimintoa eli neutraali
        keyUp('up')     #kun joystick palaa normaaliin arvoon eli se antaa numeroa 1 pythonille niin se poistaa up ja down komennon jos ne on painettune niin keysDown listasta
        keyUp('down')

    if x == 2:          #2 = kamera vasen
        keyDown('left')
        keyUp('right')
    elif x == 0:
        keyDown('right')       #kamera oikea
        keyUp('left')
    else:               #   1 = ei toimintoa eli neutraali
        keyUp('left')
        keyUp('right')

    if b == 0:          # 0 kun näppäin on painettuna
        keyDown('o')    #näppäin joystickin keskinapille joka antaa näppäimistöltä "o" kirjaimen
    else:
        keyUp('o')

        #vasen joystic
    if c == 2:              # 2 =vasen
            keyDown('a')
            keyUp('d')
    elif c == 0:               # 0 =oikea
            keyDown('d')
            keyUp('a')
    else:  # 1 =  eli neutraali releasaa painetut näppäimet
            keyUp('d')
            keyUp('a')
    if v == 0:
        keyDown('w')
        keyUp('s')
    elif v == 2:  # 0 =vasen
            keyDown('s')
            keyUp('w')
    else:  # 1 = ei toimintoa eli neutraali
        keyUp('s')
        keyUp('w')

            #näppäimet
            #antaa näppäimistöltä annetut kirjaimet jos nappia painetaan

    if p == 0:              #antaa näppäimistöltä annetut kirjaimet jos nappia painetaan
        keyDown('i')
    else:
        keyUp('i')
    if n == 0:
        keyDown('q')
    else:
        keyUp('q')
    if n2 == 0:
        keyDown('g')
    else:
        keyUp('g')
    if n3 == 0:
        keyDown('h')
    else:
        keyUp('h')
    if n4 == 0:
        keyDown('j')
    else:
        keyUp('j')
    if n5 == 0:
        keyDown('f')
    else:
        keyUp('f')
    if n6 == 0:
        keyDown('t')
    else:
        keyUp('t')
    if n7 == 0:
        keyDown('r')
    else:
        keyUp('r')
    if n8 == 0:
        keyDown('e')
    else:
        keyUp('e')

while True:                                 #ikuinen looppi datan lukemiseen ja decoodaamiseen
    rawdata = arduino.readline()            
    data = str(rawdata.decode('utf-8'))     # "," on myös yksi data esim "v1,1 " eli        V = data[0]       | 1 = data[1]         | , = data[2] jne.
    if data.startswith("v"):                #Aloittaa lukemaan tulevaa dataa jos tuleva tieto alkaa "v" kirjaimella eli tämä on data 0
        jx = int(data[1])                   #kolmas tuleva data eli data[1]
        jy = int(data[3])                   #viides tuleva data eli data[3]
        JSButton = int(data[5])

        jx2 = int(data[7])                  #vasemman joystickin luettavat arvot
        jy2 = int(data[9])
        JSBUTTON2=int(data[11])

        painike1 = int(data[13])             #painikkeiden luettavat arvot
        painike2 = int(data[15])
        painike3 = int(data[17])
        painike4 = int(data[19])
        painike5 = int(data[21])
        painike6 = int(data[23])
        painike7 = int(data[25])
        painike8 = int(data[27])

        Nappaimet(jx,jy,JSButton,jx2,jy2,JSBUTTON2, painike1,painike2,painike3,painike4,painike5,painike6,painike7,painike8)
