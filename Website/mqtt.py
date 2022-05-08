# python3.6

import random
import sqlite3

connection = sqlite3.connect("sensor.db")

from paho.mqtt import client as mqtt_client 
import time

broker = 'pitunnel.com'
port = 14079
# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 100)}'
# username = 'emqx'
# password = 'public'

cursor = connection.cursor()


def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
   # client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"`{msg.payload.decode()}` from `{msg.topic}` topic")
        ctime = int(time.time())
        print(ctime)
        if (msg.topic == "HallSensor"):
            status = msg.payload.decode()
            sql = "INSERT INTO Hall_Sensor VALUES ('{}','{}')".format(ctime, status)
            connection.execute(sql)
            connection.commit()
	
        elif (msg.topic == "GPS"):
            position = msg.payload.decode()
            posSplit = position.split(',')
            sql = "INSERT INTO GPS VALUES ('{}','{}', '{}')".format(ctime,posSplit[0], posSplit[1].strip())
            connection.execute(sql)
            connection.commit()
        
        elif (msg.topic == "HeartRateSensor"):
            heartrate = msg.payload.decode()
            sql = "INSERT INTO Heart_Rate_Sensor VALUES ('{}','{}')".format(ctime, heartrate)
            connection.execute(sql)
            connection.commit()

    client.subscribe("HallSensor")
    client.subscribe("GPS")
    client.subscribe("HeartRateSensor")
    client.subscribe("RTC")
    client.on_message = on_message


def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()

connection.close()
