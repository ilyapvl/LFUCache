/**
 * @file LFUCache.h
 * @brief Заголовочный файл для LFU кэша
 */

#ifndef LFUCACHE_H
#define LFUCACHE_H

#include <unordered_map>
#include <list>
#include <stdexcept>
#include <iostream>

#include "global.h"
#include "exceptions/CacheOperationException.h"

namespace lfu
{
    /**
     * @brief LFU кэш
     * 
     * @tparam K Тип ключа
     * @tparam V Тип значения
     */
    template<typename K, typename V>
    class LFUCache
    {
    private:
        /**
         * @brief Структура узла кэша
         */
        struct Node
        {
            K key;      
            V value;    
            int frequency;  
            
            /**
             * @brief Конструктор узла
             * @param k Ключ
             * @param v Значение
             * @param f Начальная частота
             */
            Node(const K& k, const V& v, int f) : key(k), value(v), frequency(f)
            {}
        };
        
        using NodeIterator = typename std::list<Node>::iterator;
        using SlowGetFunc = std::function<V(K)>;
        
        size_t capacity_;          
        int min_frequency_;        
        SlowGetFunc slow_get_func_;
        
        /**
         * @brief Карта частот т. е. список элементов с данной частотой
         */
        std::unordered_map<int, std::list<Node>> frequency_map_;
        
        /**
         * @brief Карта ключей - список итераторов на элементы
         */
        std::unordered_map<K, NodeIterator> key_map_;

        /**
         * @brief Увеличивает частоту использования элемента
         * @param it Итератор на элемент в списке
         * 
         * @throws CacheOperationException если элемент не найден в структурах данных
         */
        void increase_frequency(NodeIterator it);

    public:
        /**
         * @brief Конструктор кэша
         * @param capacity Вместимость кэша >0
         * @param slow_get_func Функция для медленного получения значения
         * 
         * @throws std::invalid_argument если capacity <= 0
         */
        LFUCache(size_t capacity, SlowGetFunc slow_get_func);
        
        ~LFUCache() noexcept = default;

        /**
         * @brief Получить значение по ключу
         * @param key Ключ
         * @return Ссылка на значение
         * 
         * @throws std::out_of_range если ключ не найден
         * @throws CacheOperationException если ошибка операции
         */
        V& get(const K& key);
        
        /**
         * @brief Поместить значение в кэш
         * @param key Ключ
         */
        void put(const K& key);
        
        /**
         * @brief Вытеснить один элемент из кэша
         * @throws CacheOperationException если кэш пуст
         */
        void evict();
        
        /**
         * @brief Получить текущий размер кэша
         * @return Количество элементов в кэше
         */
        size_t size() const;
        
        /**
         * @brief Проверить, пуст ли кэш
         * @return true если кэш пуст иначе false
         */
        bool empty() const;
        
        /**
         * @brief Получить вместимость кэша
         * @return Макс количество элементов
         */
        size_t capacity() const;
        
        /**
         * @brief Очистить кэш
         */
        void clear();
    };
}

#include "LFUCache.tpp"

#endif // LFUCACHE_H
