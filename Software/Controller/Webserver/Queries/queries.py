import graphene


class Query(graphene.ObjectType):
    # this defines a Field `hello` in our Schema with a single Argument `name`
    hello = graphene.String(name=graphene.String(default_value="stranger"))
    goodbye = graphene.String()
    comports = graphene.List(graphene.String)

    # our Resolver method takes the GraphQL context (root, info) as well as
    # Argument (name) for the Field and returns data for the query Response
    def resolve_hello(root, info, name):
        return f'Hello {name}!'

    def resolve_goodbye(root, info):
        return 'See ya!'

    def resolve_comports(root, info):
        return info.context.get('controller').getAllComports()
    
    def __init__(self, controller):
        self.controller = controller

