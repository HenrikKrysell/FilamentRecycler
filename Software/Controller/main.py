from flask import Flask
from flask_graphql import GraphQLView
import signal
from graphene import ObjectType, String, Schema
#import asyncio
import threading
import time

class Query(ObjectType):
    # this defines a Field `hello` in our Schema with a single Argument `name`
    hello = String(name=String(default_value="stranger"))
    goodbye = String()

    # our Resolver method takes the GraphQL context (root, info) as well as
    # Argument (name) for the Field and returns data for the query Response
    def resolve_hello(root, info, name):
        return f'Hello {name}!'

    def resolve_goodbye(root, info):
        return 'See ya!'

schema = Schema(query=Query)

app = Flask(__name__)
app.debug = True

@app.route('/')
def hello_world():
    return "hello world"

app.add_url_rule(
    '/graphql',
    view_func=GraphQLView.as_view(
        'graphql',
        schema=schema,
        graphql_schema=schema,
        graphiql=True # for having the GraphiQL interface
    )
)

# Gracefully close the app when docker closes
def handle_sigterm(*args):
    raise KeyboardInterrupt()
signal.signal(signal.SIGTERM, handle_sigterm)


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001)