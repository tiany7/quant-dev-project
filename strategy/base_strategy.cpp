#include "base_strategy.h"


void BaseStrategy::operator()(std::vector<receiver_t> &&_pipe_in, std::vector<sender_t> &&_pipe_out){
    apply_strategy(std::move(_pipe_in), std::move(_pipe_out));
}