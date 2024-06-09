#include <stdint.h>
#include <stdio.h>


#define BUFFER_SIZE (1024)
#define BUFFER_MOD (BUFFER_SIZE - 1)

typedef uint8_t buffer_type_t;

typedef struct {
    buffer_type_t buffer[BUFFER_SIZE];
    uint32_t read;
    uint32_t write;
}RingBuffer;


void ring_buffer_init(RingBuffer *rb){
	rb->write = 1;
	rb->read = 0;
}

uint32_t ring_buffer_enqueue(RingBuffer *rb, buffer_type_t data){
    uint32_t temp = (rb->write + 1) & BUFFER_MOD;
    if (rb->read == rb->write){
        return 0;
    }

    rb->buffer[rb->write] = data;
    rb->write = temp;

    return 1;
}

uint32_t ring_buffer_dequeue(RingBuffer *rb, buffer_type_t *data){

    uint32_t temp = (rb->read + 1) & BUFFER_MOD;

    if (temp == rb->write){
        return 0;
    }
    *data = rb->buffer[temp];
    rb->read = temp;

    return 1;
}

uint32_t ring_buffer_peak(RingBuffer *rb, uint32_t *data){
	*data = rb->buffer[(rb->write - 1) & BUFFER_MOD];

	return 1;
}

uint32_t ring_buffer_compare(RingBuffer *rb, const buffer_type_t *data, const uint32_t len){
    uint32_t read = (rb->read + 1) & BUFFER_MOD;
    uint32_t i = len;
    // check for buffer wrap around
    uint32_t length = (rb->read <= rb->write ? rb->write : rb->write + BUFFER_SIZE) - rb->read;
    // check if requested length is larger than buffer data
    if(len > length){
        return 0;
    }

    while(i){
        if (rb->buffer[(read + i) & BUFFER_MOD ] != data[i]) return 0;
        i--;
    }
    return 1;
}


int main(void){

    RingBuffer rb={0};
    ring_buffer_init(&rb);

    ring_buffer_enqueue(&rb, 5);
    ring_buffer_enqueue(&rb, 2);
    uint8_t data[] = {5, 2};

    printf("%i queue and data are equal\n", ring_buffer_compare(&rb, data, 2));

    uint8_t temp = 0;
    ring_buffer_dequeue(&rb, &temp);

    printf("%i queue and data are not equal\n", ring_buffer_compare(&rb, data, 2));


    return 0;
}