#FROM arm32v7/python:3
FROM python:3

ARG FOLDER_NAME=Controller

# set environment variables
ENV PYTHONDONTWRITEBYTECODE 1
ENV PYTHONUNBUFFERED 1

RUN apt-get update ##[edited]
RUN apt-get install ffmpeg libsm6 libxext6  -y

# install dependencies
RUN pip install --upgrade pip 
COPY ./Software/${FOLDER_NAME}/requirements.txt requirements.txt
RUN pip install -r requirements.txt

EXPOSE 8000

CMD ["python", "main.py"]