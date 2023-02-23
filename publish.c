#include <librdkafka/rdkafka.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    rd_kafka_t *rk;         /* Kafka producer handle */
    rd_kafka_conf_t *conf;  /* Kafka configuration */
    char *brokers;          /* Kafka broker list */
    char *topic;            /* Kafka topic to produce to */
    char *message;          /* Message to produce */
    rd_kafka_topic_t *rkt;  /* Topic handle */
    rd_kafka_resp_err_t err; /* Error code */

    /* Check arguments */
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <broker list> <topic> <message>\n", argv[0]);
        exit(1);
    }

    brokers = argv[1];
    topic = argv[2];
    message = argv[3];

    /* Create Kafka configuration */
    conf = rd_kafka_conf_new();

    /* Set the broker list */
    if (rd_kafka_conf_set(conf, "bootstrap.servers", brokers, NULL, 0) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "Failed to set broker list\n");
        exit(1);
    }

    /* Create Kafka producer */
    rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, NULL, 0);
    if (rk == NULL) {
        fprintf(stderr, "Failed to create Kafka producer\n");
        exit(1);
    }

    /* Create Kafka topic */
    rkt = rd_kafka_topic_new(rk, topic, NULL);
    if (rkt == NULL) {
        fprintf(stderr, "Failed to create Kafka topic\n");
        rd_kafka_destroy(rk);
        exit(1);
    }

    /* Produce message */
    err = rd_kafka_produce(rkt, RD_KAFKA_PARTITION_UA, RD_KAFKA_MSG_F_COPY, message, strlen(message), NULL, 0, NULL);
    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        fprintf(stderr, "Failed to produce message: %s\n", rd_kafka_err2str(err));
        rd_kafka_topic_destroy(rkt);
        rd_kafka_destroy(rk);
        exit(1);
    }

    /* Wait for messages to be delivered */
    rd_kafka_flush(rk, 10000);

    /* Destroy Kafka topic and producer */
    rd_kafka_topic_destroy(rkt);
    rd_kafka_destroy(rk);

    return 0;
}

