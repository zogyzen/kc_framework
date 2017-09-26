#pragma once

namespace KC
{
    namespace _Private
    {
        // ����
        struct TVoidClass {};

        // ȡ����
        template<unsigned i, typename TList>
        struct TGetType
        {
            typedef typename TGetType<i - 1, typename TList::list_type>::list_type list_type;
            typedef typename TGetType<i - 1, typename TList::list_type>::val_type val_type;
        };

        template<typename TList>
        struct TGetType<0, TList>
        {
            typedef TList list_type;
            typedef typename TList::val_type val_type;
        };

        // ȡ����
        template<unsigned i, typename TList>
        struct TGetListRef
        {
            static typename TGetType<i, TList>::list_type& GetList(TList& ls)
            { return TGetListRef<i - 1, typename TList::list_type>::GetList(ls.lst); }
            static const typename TGetType<i, TList>::list_type& GetListConst(const TList& ls)
            { return TGetListRef<i - 1, typename TList::list_type>::GetListConst(ls.lst); }
        };

        template<typename TList>
        struct TGetListRef<0, TList>
        {
            static TList& GetList(TList& ls) { return ls; }
            static const TList& GetListConst(const TList& ls) { return ls; }
        };

        /// ֵ�б�
        template<typename TVal, typename TList>
        struct TValList
        {
            // ��ǰֵ
            TVal val = TVal();
            // ���á��õ�ֵ
            template<unsigned i> void SetVal(const typename TGetType<i, TValList>::val_type& v)
            { TGetListRef<i, TValList>::GetList(*this).val = v; }
            template<unsigned i> typename TGetType<i, TValList>::val_type& GetVal(void)
            { return TGetListRef<i, TValList>::GetList(*this).val; }
            template<unsigned i> const typename TGetType<i, TValList>::val_type& GetVal(void) const
            { return TGetListRef<i, TValList>::GetListConst(*this).val; }

            // ����ֵ�б�
            TList lst;

            // ����
            TValList() = default;
            virtual ~TValList(void) = default;
            // ����
            TValList(const TValList& clone) : val(clone.val), lst(clone.lst) {}
            TValList& operator=(const TValList& clone) { this->val = clone.val; this->lst = clone.lst; return *this; }

            // ֵ���б�����
            typedef TVal val_type;
            typedef TList list_type;
        };

        template<typename TVal, typename TList>
        struct TValList<TVal, TValList<TVoidClass, TList>>
        {
            // ��ǰֵ
            TVal val = TVal();
            // ���á��õ�ֵ
            template<unsigned i> void SetVal(const TVal& v) { val = v; }
            template<unsigned i> TVal& GetVal(void) { return val; }
            template<unsigned i> const TVal& GetVal(void) const { return val; }

            // ����
            TValList() = default;
            virtual ~TValList(void) = default;
            // ����
            TValList(const TValList& clone) : val(clone.val) {}
            TValList& operator=(const TValList& clone) { this->val = clone.val; return *this; }

            // ֵ����
            typedef TVal val_type;
        };

        template<typename TVal>
        struct TValList<TVal, TVoidClass>
        {
            // ��ǰֵ
            TVal val = TVal();
            // ���á��õ�ֵ
            template<unsigned i> void SetVal(const TVal& v) { val = v; }
            template<unsigned i> TVal& GetVal(void) { return val; }
            template<unsigned i> const TVal& GetVal(void) const { return val; }

            // ����
            TValList() = default;
            virtual ~TValList(void) = default;
            // ����
            TValList(const TValList& clone) : val(clone.val) {}
            TValList& operator=(const TValList& clone) { this->val = clone.val; return *this; }

            // ֵ����
            typedef TVal val_type;
        };

        template<typename TList>
        struct TValList<TVoidClass, TValList<TVoidClass, TList>>
        {
            // ����
            TValList() = default;
            virtual ~TValList(void) = default;
            // ����
            TValList(const TValList& clone) = default;
            TValList& operator=(const TValList& clone) = default;
        };

        template<>
        struct TValList<TVoidClass, TVoidClass>
        {
            // ����
            TValList() = default;
            virtual ~TValList(void) = default;
            // ����
            TValList(const TValList& clone) = default;
            TValList& operator=(const TValList& clone) = default;
        };
    }

    // ֵ�б�
    template<
                typename TBaseClass, int ValListID,
                typename TVal0 = _Private::TVoidClass, typename TVal1 = _Private::TVoidClass,
                typename TVal2 = _Private::TVoidClass, typename TVal3 = _Private::TVoidClass,
                typename TVal4 = _Private::TVoidClass, typename TVal5 = _Private::TVoidClass,
                typename TVal6 = _Private::TVoidClass, typename TVal7 = _Private::TVoidClass,
                typename TVal8 = _Private::TVoidClass, typename TVal9 = _Private::TVoidClass,
                typename TVal10 = _Private::TVoidClass, typename TVal11 = _Private::TVoidClass
            >
    struct TKcTuple : public TBaseClass
    {
        // ֵ�б�
        typedef _Private::TValList<TVal0, _Private::TValList<TVal1, _Private::TValList<TVal2,
                                          _Private::TValList<TVal3, _Private::TValList<TVal4,
                                          _Private::TValList<TVal5, _Private::TValList<TVal6,
                                          _Private::TValList<TVal7, _Private::TValList<TVal8,
                                          _Private::TValList<TVal9, _Private::TValList<TVal10,
                                          _Private::TValList<TVal11,
                                          _Private::TValList<_Private::TVoidClass, _Private::TVoidClass
                                >>>>>>>>>>>>> TValListType;
        TValListType valList;

        // ��ȡֵ����ģ��
        template<unsigned i> using TGetType = _Private::TGetType<i, TValListType>;

        // ֵ��������
        unsigned ValOPType = 0;

        // ���á��õ�ֵ
        template<unsigned i>
        void SetVal(const typename TGetType<i>::val_type& v)
        { ValOPType = i; valList.SetVal<i>(v); }
        template<unsigned i>
        typename TGetType<i>::val_type& GetVal(void)
        { return valList.GetVal<i>(); }
        template<unsigned i>
        const typename TGetType<i>::val_type& GetVal(void) const
        { return valList.GetVal<i>(); }

        // ͨ��ģ�������������á��õ�ֵ��������Ӧ��ĳЩ��������֧�֣�ģ�庯����Ƕ�׵���
        template<unsigned i>
        struct TSetGetVal
        {
            TSetGetVal(TKcTuple& tp) : m_tp(tp) {}
            typedef typename TGetType<i>::val_type val_type;
            void Set(const val_type& v) { m_tp.SetVal<i>(v); }
            val_type& Get(void) { return m_tp.GetVal<i>(); }

        private:
            TKcTuple& m_tp;
        };

        // ��ȡֵ�б���
        virtual int GetID() const { return ValListID; }

        // ����
        TKcTuple() = default;
        virtual ~TKcTuple(void) = default;
        // ����
        TKcTuple(const TKcTuple& clone) : TBaseClass(clone), valList(clone.valList) {}
        TKcTuple& operator=(const TKcTuple& clone)
        { TBaseClass::operator=(clone); this->valList = clone.valList; return *this; }
    };
}
