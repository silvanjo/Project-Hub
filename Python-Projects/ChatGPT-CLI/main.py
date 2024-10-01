import openai

# Replace with your OpenAI API key
API_KEY = 'your-openai-api-key'

client = openai.OpenAI(
    # defaults to os.environ.get("OPENAI_API_KEY")
    api_key=API_KEY,
)

# Function to send a prompt to ChatGPT and get a response
def chat_gpt(prompt):
    response = client.chat.completions.create(
        model="gpt-4o-mini",
        messages=[{"role": "user", "content": prompt}]
    )
    return response.choices[0].message.content.strip()

if __name__ == "__main__":
    print("Welcome to the ChatGPT CLI! Type 'exit' or 'quit' to stop.")
    
    while True:
        # Get user input from the command line
        user_prompt = input("Enter your prompt: ")

        # Exit the loop if the user types 'exit' or 'quit'
        if user_prompt.lower() in ['exit', 'quit']:
            print("Goodbye!")
            break

        # Get response from ChatGPT
        gpt_response = chat_gpt(user_prompt)

        # Print the response from ChatGPT
        print("\nChatGPT response:", gpt_response)
        print("-" * 40)  # Divider for readability

