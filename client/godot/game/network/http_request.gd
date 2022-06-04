extends HTTPRequest

class_name HTTPRequester

signal receive(response_code, headers, body)
signal error(message)

const HOST: String = "127.0.0.1" # 127.0.0.1 | 192.168.1.71
const PORT: int = 5555
const PREFIX: String = "http"
const ENDPOINT: String = PREFIX + "://" + HOST + ":" + str(PORT)

func method_get(route: String, headers: PoolStringArray, data: String):
	request(ENDPOINT + route, headers, false, HTTPClient.METHOD_GET, data)

func method_post(route: String, headers: PoolStringArray, data: String):
	request(ENDPOINT + route, headers, false, HTTPClient.METHOD_POST, data)

func _ready():
	self.connect("request_completed", self, "_handle_request_completed")

func _handle_request_completed(result: int, response_code: int, headers: PoolStringArray, body: PoolByteArray):
	if (result == RESULT_SUCCESS):
		print(body.get_string_from_utf8())
		emit_signal("receive", response_code, headers, JSON.parse(body.get_string_from_utf8()))
	else:
		print("Error while making HTTP request to: '", ENDPOINT, "'")
		print(
			"Error code: ", result,
			", Status code: ", response_code,
			", Headers: ", headers,
			", Body: ", JSON.parse(body.get_string_from_utf8())
		)
		emit_signal("error", "Failed to send request. Try again")

