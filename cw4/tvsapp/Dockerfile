FROM node:alpine
LABEL org.opencontainers.image.authors="A47654@alunos.isel.pt"
LABEL org.opencontainers.image.authors="A50096@alunos.isel.pt"
LABEL org.opencontainers.image.authors="A50457@alunos.isel.pt"

USER root

ENV HOME=/home/node/app
WORKDIR ${HOME}
COPY ./app ${HOME}
RUN npm install && \
    chown -R node:node ${HOME}
ENV NODE_PORT=8010
EXPOSE ${NODE_PORT}
USER node
CMD ["node", "tvsapp.js"]