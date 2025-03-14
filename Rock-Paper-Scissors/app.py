from flask import Flask, render_template, request
import random

# Create a Flask app
app = Flask(__name__)

# Define choices
choices = ['rock', 'paper', 'scissors']

# Function to determine the winner
def determine_winner(user_choice, server_choice):
    if user_choice == server_choice:
        return 'It\'s a tie!'
    elif (user_choice == 'rock' and server_choice == 'scissors') or \
         (user_choice == 'paper' and server_choice == 'rock') or \
         (user_choice == 'scissors' and server_choice == 'paper'):
        return 'You win!'
    else:
        return 'You lose!'

# Define the homepage route
@app.route('/')
def home():
    return render_template('index.html')

# Define the game route
@app.route('/play', methods=['POST'])
def play():
    user_choice = request.form['choice']
    server_choice = random.choice(choices)
    result = determine_winner(user_choice, server_choice)
    return render_template('result.html', user_choice=user_choice, server_choice=server_choice, result=result)

# Run the app
if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
