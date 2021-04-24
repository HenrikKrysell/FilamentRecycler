# BUILD
FROM node:14 as build

ARG COMPONENT_NAME=frontend

ENV GENERATE_SOURCEMAP=false
ENV TZ=Europe/Stockholm

WORKDIR /usr/app/src

COPY package.json .
COPY yarn.lock .
COPY packages/ packages/

RUN yarn workspace $COMPONENT_NAME install
RUN yarn workspace $COMPONENT_NAME run build


# PRODUCTION
FROM nginx:1.19.0-alpine

ARG FOLDER_NAME=Frontend

COPY packages/$FOLDER_NAME/nginx.config /etc/nginx/conf.d/default.conf
COPY --from=build /usr/app/src/packages/$FOLDER_NAME/build /usr/share/nginx/html

WORKDIR /usr/share/nginx/html

EXPOSE 80

# Add bash
RUN apk add --no-cache bash

# Make our shell script executable
RUN chmod +x env.sh

# Start Nginx server
CMD ["/bin/bash", "-c", "/usr/share/nginx/html/env.sh && nginx -g \"daemon off;\""]