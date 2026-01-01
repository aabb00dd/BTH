from flask import Flask, jsonify, render_template , request
import requests

app = Flask(__name__)

URL = 'secret_url_here'  # Replace with the actual URL to fetch data


@app.route("/")
def hello_world():
    return render_template("home_page.html")


def fetch_data():
    response = requests.get(URL)
    return response.json().get('data', [])


def get_unique_products(data):
    unique_products = {}

    for product in data:
        name = product.get('name', '')
        stock = int(str(product.get('stock', '0')).isdigit() and product.get('stock', '0'))
        unique_products.setdefault(name, {'name': name, 'stock': 0})
        unique_products[name]['stock'] += stock

    return list(unique_products.values())


@app.route('/unique', methods=['GET'])
def unique():
    data = fetch_data()
    return render_template("products.html", products=jsonify({'data': get_unique_products(data)}).get_json())


@app.route('/search', methods=['GET'])
def search():
    query = request.args.get('query', '')
    data = fetch_data()

    filtered_data = [product for product in data if query.upper() in product.get('name', '').upper()]
    return render_template("product_search.html", srch=jsonify({'data': get_unique_products(filtered_data)}).get_json())


def filter_tags(tags):
    return [tag for tag in tags if tag.get('confidence', 0) >= 0.97]


def translate_tags(tags):
    translation_url = "secret_translation_url_here"  # Replace with the actual translation URL
    headers = {"secret": "key_here"}  # Replace with the actual header key
    texts_to_translate = [{"text": tag.get("name", "")} for tag in tags]

    response = requests.post(translation_url, headers=headers, json=texts_to_translate)
    response_data = response.json()
    if response.status_code != 200:
        return render_template("image_search.html", error_message=response_data["error"])

    translations = response.json()
    if isinstance(translations, list) and translations:
        translated_tags = [
            {"name": translation.get("translations", [])[0].get("text", ""), "stock": tag.get("stock", 0)}
            for tag, translation in zip(tags, translations)
        ]
        return translated_tags

@app.route('/image_search')
def image_search():
    image_url = request.args.get("image-url", "")
    tags = []
    filtered_tags = []
    translated_tags = []
    filtered_data = []
    translated_data = []

    if image_url:
            api_url = "secret_api_url_here"  # Replace with the actual API URL for image analysis
            headers = {"secret": "key_here"}  # Replace with the actual header key
            image_url_data = {"url": image_url}

            response = requests.post(api_url, headers=headers, json=image_url_data)
            print(response)
            print(response.json())
            response_data = response.json()
            if response.status_code != 200:
                return render_template("image_search.html", error_message=response_data)

            tags = response.json().get("tags", [])
            data = fetch_data()
            filtered_tags = filter_tags(tags)
            translated_tags = translate_tags(filtered_tags)

            filtered_tag_names = [tag.get('name', '') for tag in filtered_tags]
            filtered_data = [product for product in data if any(tag_name in product.get('name', '').lower() for tag_name in filtered_tag_names)]

            for product in data:
                for tag in translated_tags:
                    translated_name = tag.get('name', '').lower()
                    product_name = product.get('name', '').lower()
                    if translated_name in product_name or product_name in translated_name:
                        translated_data.append(product)

    return render_template("image_search.html", image_url=image_url, tags=filtered_data, translated_tags=translated_data, error_message=None)


if __name__ == '__main__':
    app.run(debug=True)
