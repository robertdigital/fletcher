// Copyright 2018-2019 Delft University of Technology
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>
#include <functional>
#include <utility>
#include <variant>
#include <memory>
#include <vector>

#include "dag/types.h"

#pragma once

namespace dag {

struct VertexProfile {
  uint32_t elements;
  uint32_t valids;
  uint32_t readies;
  uint32_t transfers;
  uint32_t packets;
  uint32_t cycles;
};

typedef std::vector<VertexProfile> ProfileParam;

typedef std::function<std::string(ProfileParam)> ProfileParamFunc;
typedef std::variant<std::string, ProfileParamFunc> AnyParamFunc;

struct Constant {
  Constant(std::string name, AnyParamFunc value) : name(std::move(name)), value(std::move(value)) {}
  std::string name;
  AnyParamFunc value;

  Constant &operator=(AnyParamFunc func);
};

std::shared_ptr<Constant> constant(const std::string &name, const AnyParamFunc &value);

struct Transform;

struct Vertex {
  Vertex(std::string name, TypeRef type) : name(std::move(name)), type(std::move(type)) {}
  virtual ~Vertex() = default;
  std::string name;
  TypeRef type;
  [[nodiscard]] virtual bool IsInput() const = 0;
  [[nodiscard]] virtual bool IsOutput() const = 0;
};

struct In : public Vertex {
  In(std::string name, TypeRef type) : Vertex(std::move(name), std::move(type)) {}
  [[nodiscard]] bool IsInput() const override { return true; }
  [[nodiscard]] bool IsOutput() const override { return false; }
};

struct Out : public Vertex {
  Out(std::string name, TypeRef type) : Vertex(std::move(name), std::move(type)) {}
  [[nodiscard]] bool IsInput() const override { return false; }
  [[nodiscard]] bool IsOutput() const override { return true; }
};

std::shared_ptr<In> in(const std::string &name, const TypeRef &type);
std::shared_ptr<Out> out(const std::string &name, const TypeRef &type);

struct Edge {
  Edge(const Vertex *dst, const Vertex *src);
  std::string name;
  const Vertex *src_;
  const Vertex *dst_;

  Edge &named(std::string name);
};

struct Transform {
  std::string name;
  std::vector<std::shared_ptr<Constant>> constants;
  std::vector<std::shared_ptr<In>> inputs;
  std::vector<std::shared_ptr<Out>> outputs;

  // placeholder for something more elaborate:
  bool reads_memory = false;
  bool writes_memory = false;

  [[nodiscard]] Constant &c(const std::string &constant_name) const;
  [[nodiscard]] const Vertex &o(size_t i) const;
  [[nodiscard]] const Vertex &i(size_t i) const;

  const Vertex &operator()(const std::string &vertex_name) const;

  Transform &operator+=(const std::shared_ptr<Constant> &c);
  Transform &operator+=(const std::shared_ptr<Vertex> &v);

  [[nodiscard]] bool Has(const Vertex &v) const;

  [[nodiscard]] std::string ToString() const;
  [[nodiscard]] std::string ToStringInputs() const;
  [[nodiscard]] std::string ToStringOutputs() const;
};

void operator<<(const Constant &dst, const std::string &src);

Edge operator<<(const Vertex &dst, const Vertex &src);
Edge operator<<(const Transform &dst, const Vertex &src);
Edge operator<<(const Vertex &dst, const Transform &src);
Edge operator<<(const Transform &dst, const Transform &src);

struct Graph {
  std::string name = "FletcherDAG";
  std::vector<std::shared_ptr<Transform>> transformations;
  std::vector<std::shared_ptr<Edge>> edges;

  const Transform &Add(const Transform &t);
  const Edge &Add(const Edge &e);

  const Transform &operator<<=(const Transform &t);
  const Edge &operator<<=(const Edge &e);

  [[nodiscard]] const Transform &ParentOf(const Vertex &v) const;
};

}  // namespace dag