from flask import Flask, request, jsonify, send_from_directory
import subprocess
import os

app = Flask(__name__, static_url_path='', static_folder='static')

@app.route('/')
def serve_index():
    return send_from_directory('static', 'index.html')

@app.route('/run', methods=['POST'])
def run_code():
    code = request.json.get('code')
    with open('program.nbl', 'w') as f:
        f.write(code)
    
    try:
        cwd = os.getcwd()
        binary_path = cwd + '/bin/nimble'
        result = subprocess.run([binary_path, 'program.nbl'], capture_output=True, text=True, timeout=5)
        output = result.stdout
        error = result.stderr
    except subprocess.TimeoutExpired:
        output = ""
        error = "[ERROR] Execution timed out"
    
    os.remove('program.nbl')
    
    return jsonify({'output': output, 'error': error})

if __name__ == '__main__':
    app.run(debug=True)
