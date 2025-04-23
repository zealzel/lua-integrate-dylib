# py_client.py
import grpc
import hello_pb2
import hello_pb2_grpc


def run():
    channel = grpc.insecure_channel("localhost:50051")
    stub = hello_pb2_grpc.HelloServiceStub(channel)
    resp = stub.SayHello(hello_pb2.HelloRequest(name="Kevin"))
    print("Client received:", resp.message)


if __name__ == "__main__":
    run()
