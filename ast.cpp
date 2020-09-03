#include "ast.hpp"

auto PrintStatement::accept(Visitor& visitor) -> void { visitor.visit(*this); }

auto InputStatement::accept(Visitor& visitor) -> void { visitor.visit(*this); }

auto ShiftLeftStatement::accept(Visitor& visitor) -> void { visitor.visit(*this); }

auto ShiftRightStatement::accept(Visitor& visitor) -> void { visitor.visit(*this); }

auto LoopStatement::accept(Visitor& visitor) -> void { visitor.visit(*this); }

auto IncrementStatement::accept(Visitor& visitor) -> void { visitor.visit(*this); }

auto DecrementStatement::accept(Visitor& visitor) -> void { visitor.visit(*this); }
