FROM python:3

ARG FOLDER_NAME=Controller

# set environment variables
ENV PYTHONDONTWRITEBYTECODE 1
ENV PYTHONUNBUFFERED 1

# install dependencies
RUN pip install --upgrade pip 
COPY ./Software/${FOLDER_NAME}/requirements.txt requirements.txt
RUN pip install -r requirements.txt

EXPOSE 5001

CMD ["python", "main.py"]