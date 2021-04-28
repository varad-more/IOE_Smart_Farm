from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
def home():
    return render_template('home.html')

@app.route('/ldr')
def ldr():
    return render_template('ldr.html')

@app.route('/temperature')
def temperature():
    return render_template('temperature.html')

@app.route('/humidity')
def humidity():
    return render_template('humidity.html')

@app.route('/motor')
def motor():
    return render_template('motor.html')

@app.route('/soil_moisture')
def soil_moisture():
    return render_template('soil_moisture.html')

if __name__ == '__main__':
    app.run(debug=True)