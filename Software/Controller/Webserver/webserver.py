from flask import Flask
from flask_graphql import GraphQLView
from graphene import Schema
from Webserver.Queries.queries import Query
from flask import g

def get_db():
    return "pelle"

class Webserver:

  def run(self, controller):
    schema = Schema(query=Query)
    app = Flask(__name__)
    app.debug = True
    app.add_url_rule(
        '/graphql',
        view_func=GraphQLView.as_view(
            'graphql',
            schema=schema,
            graphql_schema=schema,
            graphiql=True, # for having the GraphiQL interface
            #context="pellekorv"
            get_context=lambda: {'controller': controller}
        )
    )

    app.run(host='0.0.0.0', port=5001, use_reloader=False)
