FROM node:14

ARG FOLDER_NAME=Gateway
ARG env=prod

ENV NODE_ENV=${env}

EXPOSE 3000

CMD [ "npm", "run", "start-production"]