from flask import Flask, jsonify, request
import tensorflow as tf
import numpy as np
from PIL import Image
import os

# Load Model
model = tf.keras.models.load_model("helmet_model (1).h5")

app = Flask(__name__)

def preprocess_image(image_path):
    image = Image.open(image_path).resize((64, 64)).convert('RGB')
    image = np.array(image) / 255.0
    image = np.expand_dims(image, axis=0)
    return image

@app.route('/predict', methods=['GET'])
def predict():
    try:
        # Path to the image in the static folder
        image_path = "static/images/BikesHelmets17.png"
        
        # Preprocess image
        processed_image = preprocess_image(image_path)

        # Predict
        prediction = model.predict(processed_image)
        label = "Helmet" if prediction[0][0] >= 0.5 else "No Helmet"

        return jsonify({"prediction": label})

    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    port = int(os.environ.get("PORT", 10000))  # Render assigns a dynamic port
    app.run(host="0.0.0.0", port=port, debug=False)
