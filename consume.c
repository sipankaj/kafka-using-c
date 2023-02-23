#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <librdkafka/rdkafka.h>

static volatile sig_atomic_t run = 1;

static void sigterm(int sig) {
    run = 0;
}

int main(int argc, char **argv) {
    rd_kafka_t *rk;
    rd_kafka_conf_t *conf;
    rd_kafka_topic_partition_list_t *topic_list;
    char errstr[512];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <broker> <topic>\n", argv[0]);
        return 1;
    }

    /* Create configuration objects */
    conf = rd_kafka_conf_new();

    /* Set configuration properties */
    rd_kafka_conf_set(conf, "bootstrap.servers", argv[1], errstr, sizeof(errstr));
    rd_kafka_conf_set(conf, "group.id", "my-group", errstr, sizeof(errstr));

    /* Create consumer instance */
    rk = rd_kafka_new(RD_KAFKA_CONSUMER, conf, errstr, sizeof(errstr));
    if (!rk) {
        fprintf(stderr, "Failed to create consumer: %s\n", errstr);
        rd_kafka_conf_destroy(conf);
        return 1;
    }

    /* Create topic partition list and add topic */
    topic_list = rd_kafka_topic_partition_list_new(1);
    rd_kafka_topic_partition_list_add(topic_list, argv[2], RD_KAFKA_PARTITION_UA);

    /* Subscribe to topic */
    if (rd_kafka_subscribe(rk, topic_list) != RD_KAFKA_RESP_ERR_NO_ERROR) {
        fprintf(stderr, "Failed to subscribe to topic: %s\n", rd_kafka_err2str(rd_kafka_last_error()));
        rd_kafka_topic_partition_list_destroy(topic_list);
        rd_kafka_destroy(rk);
        rd_kafka_conf_destroy(conf);
        return 1;
    }

    /* Set up signal handler for clean shutdown */
    signal(SIGINT, sigterm);
    signal(SIGTERM, sigterm);

    /* Consume messages */
    while (run) {
        rd_kafka_message_t *msg;

        msg = rd_kafka_consumer_poll(rk, 1000);
        if (msg) {
            if (msg->err == RD_KAFKA_RESP_ERR_NO_ERROR) {
                printf("Received message (%zd bytes): %.*s\n", msg->len, (int)msg->len, (char *)msg->payload);
            } else {
                fprintf(stderr, "Failed to consume message: %s\n", rd_kafka_err2str(msg->err));
            }
            rd_kafka_message_destroy(msg);
        }
    }

    /* Clean up */
    rd_kafka_topic_partition_list_destroy(topic_list);
    rd_kafka_unsubscribe(rk);
    rd_kafka_destroy(rk);
    rd_kafka_conf_destroy(conf);

    return 0;
}

