from flask import Flask, render_template_string

app = Flask(__name__)

@app.route("/")
@app.route("/<path:path>")
def hello(path=None):
	if not path:
		body = "Visit hello.tghack.no/your_name"
	else:
		body = "Hello {}!".format(path)

	template_str = """<html>
	<head><body>
		{}
	</body></head>
	</html>""".format(body)

	return render_template_string(template_str)

if __name__ == "__main__":
	with open("flag.txt", "r") as f:
		flag = f.read()
	app.config.from_envvar("APPLICATION_SETTINGS")
	app.run()
