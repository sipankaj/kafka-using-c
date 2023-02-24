# Kafka C Producer and Consumer Example

This is a simple C program that demonstrates how to produce and consume Kafka events using the `librdkafka` library. This example shows how to create a Kafka producer that publishes messages to a topic, and a Kafka consumer that subscribes to the same topic and receives the messages.

## Prerequisites

Before running this program, you should have the following:

- A Kafka cluster set up and running
- The `librdkafka` library installed on your system

## Installing librdkafka

You can install the `librdkafka` library using the package manager for your operating system, or by building it from source. Refer to the [librdkafka documentation](https://github.com/edenhill/librdkafka#building-and-installing) for detailed instructions on how to install the library.

## Building the Program

To build the program, run the following command:

gcc -o publish publish.c -lrdkafka
gcc -o consume consume.c -lrdkafka


This will compile the `publish.c` and `consume.c` files and link them with the `librdkafka` library.

## Running the Producer

To run the producer, use the following command:
./publish <broker_list> <topic_name> <message>

Replace `<broker_list>` with a comma-separated list of Kafka brokers, and `<topic_name>` with the name of the topic you want to produce events to.

For example:
./publish localhost:9092 test_topic hello

This will create a Kafka producer that connects to the brokers specified in `<broker_list>` and produces messages to the `test_topic` topic.

## Running the Consumer

To run the consumer, use the following command:

./consume <broker_list> <topic_name>

Replace `<broker_list>` with a comma-separated list of Kafka brokers, `<topic_name>` with the name of the topic you want to consume events from.

For example:

./consume localhost:9092 test_topic

This will create a Kafka consumer that connects to the brokers specified in `<broker_list>`, subscribes to the `test_topic` topic. The consumer will print out any messages it receives from the topic.

## Conclusion

This example demonstrates how to use the `librdkafka` library to create a simple Kafka producer and consumer in C. By modifying the code, you can customize the behavior of the producer and consumer to suit your needs.
