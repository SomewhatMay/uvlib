#pragma once

/**
 * Enables the singleton architecture.
 */

template <typename T>
class Singleton {
 public:
  // Delete copy constructor and copy assignment
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;

  // Delete move constructor and move assignment
  Singleton(Singleton&&) = delete;
  Singleton& operator=(Singleton&&) = delete;

  // Provide global access to the singleton instance
  static T& get_instance() {
    static T instance;
    return instance;
  }

 protected:
  // Constructor is protected to allow derived classes to initialize
  Singleton() = default;
  ~Singleton() = default;
};