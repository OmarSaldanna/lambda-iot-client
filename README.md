# Lambda IoT Client
_bellow there's an english documentation_

El Lambda Iot Client es una aplicación para computadoras que permite comunicar dispositivos Arduino con el Lambda. Este es básicamente un programa de Python que se conecta a un chat que permite comunicar el servidor principal de Lambda con dispositivos Arduino para **interactuar con ellos desde un chat, desde cualquier lugar**. En este sistema podrás conectar:

> **Dispositivos de Salida:** se pueden conectar leds, relevadores, displays LCD y más.
> **Dispositivos de Entrada:** sensores, potenciómetros y más.

Este sistema permite encender, apagar, mandar y leer valores de los Arduino, he aquí algunos ejemplos de acciones:

* Encender y Apagar Leds
* Manipular Relevadores
* Leer el estado de sensores
* Leer el estado de dispositivos
* Cambiar el texto de pantallas LCD
* Cambiar el valor de variables

## Instalación :key:

1. Lo primero es descargar el código mediante git o descargándolo directamente como un Zip.

2. Después crea un ambiente virtual de python dentro del folder **lambda-iot-client**, ejecuta los siguientes comandos en la terminal **dentro del folder del proyecto**:

```bash
python -m venv
pip install -r requirements.txt
```
> Nota: Puede que tu terminal no te reconozca el comando **python** o **pip** de ser así, utiliza los comandos **pip3** y **python3**. Este error suele suceder en *MacOS y Linux*.

3. Finalmente deberás de localizar el archivo `data.json` que se encuentra **dentro del folder llamado db** que es el que contiene los datos de conexión. En este deberás colocar: **la IP de Lambda, puerto del chat y tu id de discord**; dichos datos se los deberás de pedir a un administrador de Lambda mediante Discord. Tu id te la puede brindar Lambda, simplemente dile `"Lambda dame mi id"`.

> Nota: recuerda que este archivo sigue un formato **json**, de haber un error en la sintaxis la aplicación marcará un error. Si deseas saber si tu archivo está **correctamente escrito** preguntale a Lambda si la sintaxis de tu archivo es correcta, pasándole el contenido del archivo.

## Configuraciones :gear:
Ya habiendo instalado las librerías necesarias para que la app funcione y las configuraciones de conexión con Lambda, hace falta agregar tus **dispositivos**. Estos serán puestos en el archivo de `data.json`. He aquí un ejemplo de como agregar los dispositivos:

```json
"devices": {
	"led rojo": {
		"ip": "192.168.1.65"
		"type": "output"
	},
	"led verde": {
		"ip": "192.168.1.64"
		"type": "output"
	},
}
```
* Puedes ponerle el nombre desees a los dispositivos, por ejemplo: "luz de afuera", "luz del cuarto", "termómetro", "led azul". Los nombres de los dispositivos pueden contener acentos, pero no caracteres especiales, como puntos, comas, etc.

* Después hay que obtener la IP del Arduino, entonces a cada dispositivo se le debe de asignar la IP del Arduino en el que está. **Puede repetirse la IP**, esto implicaría que **hay dos dispositivos conectados al mismo Arduino** (igualmente pueden ser más de dos). Si la **IP es diferente**, este caso sería para cuando los dispositivos están en diferente Arduino. Ambos casos están bien, pues **este sistema puede soportar múltiples Arduinos conectados y todos los dispositivos que se quieran**.

* Para el "type" o tipo, solamente puede tomar los valores de **"output" o "input"**, y como sus nombres lo sugieren el **output es para dispositivos de salida** y el **input para dispositivos de entrada**. Por ejemplo:

* **Output**: Leds, Releadores, Motores, Pantallas OLED, Displays LCD y más.
* **Input**: Sensores, Botones, Palancas, Potenciómetros y semejantes.

> Nota: el **type** es meramente una formalidad, pues al momento de programar los Arduinos tendrás **completa libertad de qué hacer con tus dispositivos**. Además, aunque un dispositivo se defina como de salida (output), igualmente se puede leer su estado como un input. Dentro de la carpeta de `arduino examples` encontrarás un programa que emplea este ejemplo, llamado `"write-n-read-led.ino"`. **Dentro de los ejemplos de Arduino encontrarás las configuraciones de los dispositivos para que solamente los copies y pegues.**

## Siguientes Pasos :robot:

En base a los exemplos en Arduino podrás crear tus propios sistemas de Internet de las Cosas con todos los dispositivos que quieras, podrías conectar algunos dispositivos de tu casa y controlarlos desde el chat de Lambda, desde donde sea. 