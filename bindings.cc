#include <v8.h>
#include <node.h>
#include "heatmap.h"
#include "lodepng.h"
#include <node_buffer.h>


using namespace v8;
using namespace node;

namespace heatmap {
    Handle<Value> Create(const Arguments& args) {
        HandleScope scope;
        if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsArray()) {
            ThrowException(Exception::TypeError(String::New("You must specify a width, height and data points.")));
            return scope.Close(Undefined());
        }

        Local<Number> width = Number::New(args[0]->NumberValue());
        Local<Number> height = Number::New(args[1]->NumberValue());

        Local<Number> radius = Number::New(args[2]->NumberValue());

        heatmap_stamp_t* stamp = heatmap_stamp_gen(radius->Value());
        heatmap_t* hm = heatmap_new(width->Value(), height->Value());
        v8::Local<v8::Array> datapoints = v8::Local<v8::Array>::Cast(args[3]);

        for (uint32_t i = 0; i < datapoints->Length();i++) {
            v8::Local<v8::Array> point = v8::Local<v8::Array>::Cast(datapoints->Get(i));

            double x = point->Get(0)->NumberValue();
            double y = point->Get(1)->NumberValue();
            heatmap_add_point_with_stamp(hm, x, y, stamp);
        }
        int arraySize = (int) width->Value() * height->Value() * 4;
        unsigned char image[arraySize];

        heatmap_render_default_to(hm, image);

        unsigned char* png;
        size_t pngsize;
        unsigned error = lodepng_encode32(&png, &pngsize, image, width->Value(), height->Value());
        if (error)
            return Number::New(error);

        Buffer *slowBuffer = node::Buffer::New(pngsize);
        memcpy(Buffer::Data(slowBuffer), png, pngsize);

        Local<Object> globalObj = Context::GetCurrent()->Global();
        Local<Function> bufferConstructor = Local<Function>::Cast(globalObj->Get(String::New("Buffer")));
        Handle<Value> constructorArgs[3] = { slowBuffer->handle_, v8::Integer::New(pngsize), v8::Integer::New(0) };
        Local<Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);
        
        heatmap_free(hm);
        heatmap_stamp_free(stamp);
        
        return scope.Close(actualBuffer);
        //return Boolean::New(true);
    }

    void Initialize(Handle<Object> exports) {
        exports->Set(String::NewSymbol("create"),
            FunctionTemplate::New(Create)->GetFunction());

    }

}

NODE_MODULE(heatmap, heatmap::Initialize);
