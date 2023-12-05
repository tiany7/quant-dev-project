#pragma once

#include <any>
#include <memory>
#include <vector>
#include <future>
#include <functional>

#include "utils.h"
#include "data_manager.h"

using outgoing_pipe_t = std::shared_ptr<utils::mpsc::Sender<std::any>>;
using incoming_pipe_t = std::unique_ptr<utils::mpsc::Receiver<std::any>>;

class BaseTransformer {
public:
    BaseTransformer() = default;
    virtual ~BaseTransformer() = default;
    virtual void transform(std::vector<incoming_pipe_t> &&pipe_in, std::vector<outgoing_pipe_t> &&pipe_out) = 0;
    void operator()(std::vector<incoming_pipe_t> &&_pipe_in, std::vector<outgoing_pipe_t> &&_pipe_out){
    
    } // declare it as pure virtual function
};


class ValidateTransformer : public BaseTransformer {
public:
    ValidateTransformer() = default;
    virtual ~ValidateTransformer() = default;
    void operator()(std::vector<incoming_pipe_t> &&pipe_in, std::vector<outgoing_pipe_t> &&pipe_out){
        this->transform(std::move(pipe_in), std::move(pipe_out));
    }

    virtual void transform(std::vector<incoming_pipe_t> &&pipe_in, std::vector<outgoing_pipe_t> &&pipe_out){
        // checks for condition
        // if condition is not met, directly throw
        // std::cerr<<"validate entered"<<std::endl;
        for(auto data = pipe_in[0]->receive(); !is_err(data); data = pipe_in[0]->receive()){
            // std::cerr<<pipe_in[0].ref_count()<<std::endl;
            auto &&item = std::get<std::any>(data);
            auto &&real_item = std::any_cast<MarketData>(item);
            // std::cerr<<"price: "<<real_item.price<<std::endl;
            if(real_item.price < 0){
                throw std::runtime_error("price should be positive");
            }
            pipe_out[0]->send(real_item);
        }
        std::cerr<<"validate finished\n";
        // reduce ref_count of pipe_in[0] by one
        std::cerr<<pipe_out[0].use_count()<<std::endl;
        // pipe_out[0].reset();

    }
};


class AugmentTransformer : public BaseTransformer {
public:
    AugmentTransformer() = default;
    virtual ~AugmentTransformer() = default;

    void operator()(std::vector<incoming_pipe_t> &&pipe_in, std::vector<outgoing_pipe_t> &&pipe_out){
        this->transform(std::move(pipe_in), std::move(pipe_out));
    }

    virtual void transform(std::vector<incoming_pipe_t> &&pipe_in, std::vector<outgoing_pipe_t> &&pipe_out){
        // checks for condition
        // if condition is not met, directly throw
        // std::cerr<<"augment entered"<<std::endl;
        for(auto data = pipe_in[0]->receive(); !is_err(data); data = pipe_in[0]->receive()){
            // std::cerr<<pipe_in[0].ref_count()<<std::endl;
            // std::cerr<<"received any"<<std::endl;
            auto &&item = std::get<std::any>(data);
            auto &&real_item = std::any_cast<MarketData>(item);
            
            real_item.price += 100;
            if(real_item.price < 0){
                throw std::runtime_error("price should be positive");
            }
            pipe_out[0]->send(real_item);
        }
        std::cerr<<"augment finished\n";
        // pipe_out[0].reset();
    }
};


class TransformerManager {
private:
    // std::vector<std::shared_future<void>> handles;
public:
    TransformerManager() = default;
    ~TransformerManager() = default;

    TransformerManager& apply(std::function<void(std::vector<incoming_pipe_t> &&, std::vector<outgoing_pipe_t> &&)> transformer, std::vector<incoming_pipe_t> &&pipe_in, std::vector<outgoing_pipe_t> &&pipe_out){
        std::thread t(transformer, std::move(pipe_in), std::move(pipe_out));
        t.detach();
        std::cerr<<"thread joined"<<std::endl;
        return *this;
    }

    // void wait(){
    //     for(auto &handle : handles){
    //         handle.wait();
    //     }
    // }

};