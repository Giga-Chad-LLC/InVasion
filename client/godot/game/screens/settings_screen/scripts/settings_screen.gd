extends Control

signal scene_changed(scene_name)
signal play_click_sound()

onready var Host = $CenterContainer/Form/HostInput
onready var TcpPort = $CenterContainer/Form/TcpPortInput
onready var HttpPort = $CenterContainer/Form/HttpPortInput

func _ready():
	Host.text = Global.NETWORK_HOST
	TcpPort.text = str(Global.TCP_PORT)
	HttpPort.text = str(Global.HTTP_PORT)

func _on_NextButton_pressed():
	Global.NETWORK_HOST = Host.text
	Global.TCP_PORT = int(TcpPort.text)
	Global.HTTP_PORT = int(HttpPort.text)
	
	emit_signal("play_click_sound")
	emit_signal("scene_changed", "start_screen")
