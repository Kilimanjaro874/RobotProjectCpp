#pragma once
#include <memory>
#include <functional>
#include "tnl_shared_factory.h"


namespace tnl {

/*
//
//  �g�p�@�T���v��
//

class Test : public tnl::link_linear {
public :
    using shared = std::shared_ptr<Test>;
    Test(int n) : n_(n) {}
    int n_;
};

Test::shared t;

void gameMain(float deltatime) {
    if (!init) {
        srand(time(0));
        t = std::make_shared<Test>( rand()%50 );
        for (int i = 0; i < 30; ++i) {
            t->pushBack(std::make_shared<Test>(rand()%50));
        }
        init = true;
    }
    t = tnl::link_linear::SortAscending<Test, int>(t, [](tnl::link_linear::shared obj) {
            return std::static_pointer_cast<Test>( obj )->n_; });
}
*/

	//-------------------------------------------------------------------------------------------------------------------
	//
	// �o�������`���X�g�\��
	// tips... �p�������Ďg�p���邱�Ƃ�z�肵�����X�g�N���X
	// tips... ���̃N���X���p�������ꍇ shared_ptr �ł������p�ł��܂���
	// tips... �z�Q�Ƃ�����邽�� prev �� weak_ptr �ɂȂ��Ă��܂��A�擪�m�[�h�͏�ɕێ����Ă�������
    //
    class link_linear : public tnl::SharedFactory<link_linear> {
    public:
        using weak = std::weak_ptr<link_linear>;
        using shared = std::shared_ptr<link_linear>;

    protected:
        link_linear() {}

    private:
        weak prev_;
        shared next_ = nullptr;

        static shared Swap(shared a, shared b, shared& front) {
            shared ap, an;
            shared bp, bn;
            shared sa = a;
            shared sb = b;
            if (a->getPrev()) ap = a->getPrev();
            if (a->getNext()) an = a->getNext();
            if (b->getPrev()) bp = b->getPrev();
            if (b->getNext()) bn = b->getNext();
            if (an == b) {
                if (ap) ap->next_ = b;
                if (bn) bn->prev_ = a;
                a->prev_ = b;
                a->next_ = bn;
                b->prev_ = ap;
                b->next_ = a;
                front = (b->getPrev()) ? front : b;
                return front;
            }
            if (bn == a) {
                if (an) an->prev_ = b;
                if (bp) bp->next_ = a;
                a->prev_ = bp;
                a->next_ = b;
                b->prev_ = a;
                b->next_ = an;
                front = (a->getPrev()) ? front : a;
                return front;
            }
            if (ap) ap->next_ = b;
            if (an) an->prev_ = b;
            if (bp) bp->next_ = a;
            if (bn) bn->prev_ = a;
            a->prev_ = bp;
            a->next_ = bn;
            b->prev_ = ap;
            b->next_ = an;
            front = (a->getPrev()) ? front : a;
            front = (b->getPrev()) ? front : b;
            return front;
        }

        template< class CompType >
        static shared Sort_ascending(shared& old_front, shared s, shared e, const std::function<CompType(shared)>& comp_vlue) {
            if (s == e) return old_front;

            CompType pivot = comp_vlue(s);

            shared p1 = s;
            shared p2 = e;
            shared new_front;

            while (1) {
                if (comp_vlue(p1) >= pivot && comp_vlue(p2) < pivot) {
                    if (s == p1) s = p2;
                    if (e == p2) e = p1;
                    new_front = Swap(p1, p2, old_front);
                    std::swap(p1, p2);
                }
                if (comp_vlue(p1) < pivot) {
                    p1 = p1->getNext();
                    if (p1 == p2) { break; }
                }
                if (comp_vlue(p2) >= pivot) {
                    p2 = p2->getPrev();
                    if (p1 == p2) { break; }
                }
            }

            if (p1->getPrev() && p1 != s && comp_vlue(p1) >= pivot) p1 = p1->getPrev();
            else if (p2->getNext() && p2 != e) p2 = p2->getNext();

            new_front = Sort_ascending(old_front, s, p1, comp_vlue);
            new_front = Sort_ascending(old_front, p2, e, comp_vlue);

            return new_front;
        }

        template< class CompType >
        static shared Sort_descending(shared& old_front, shared s, shared e, const std::function<CompType(shared)>& comp_vlue) {
            if (s == e) return old_front;

            CompType pivot = comp_vlue(s);

            shared p1 = s;
            shared p2 = e;
            shared new_front;

            while (1) {
                if (comp_vlue(p1) <= pivot && comp_vlue(p2) > pivot) {
                    if (s == p1) s = p2;
                    if (e == p2) e = p1;
                    new_front = Swap(p1, p2, old_front);
                    std::swap(p1, p2);
                }
                if (comp_vlue(p1) > pivot) {
                    p1 = p1->getNext();
                    if (p1 == p2) { break; }
                }
                if (comp_vlue(p2) <= pivot) {
                    p2 = p2->getPrev();
                    if (p1 == p2) { break; }
                }
            }

            if (p1->getPrev() && p1 != s && comp_vlue(p1) <= pivot) p1 = p1->getPrev();
            else if (p2->getNext() && p2 != e) p2 = p2->getNext();

            new_front = Sort_descending(old_front, s, p1, comp_vlue);
            new_front = Sort_descending(old_front, p2, e, comp_vlue);

            return new_front;
        }


    public:

        virtual ~link_linear() {}

        //-----------------------------------------------------------------------------------------------------
        // �O�̎擾
        inline shared getNext() const noexcept {
            return next_;
        }
        //-----------------------------------------------------------------------------------------------------
        // ���̎擾
        inline shared getPrev() const noexcept {
            return prev_.lock();
        }

        //-----------------------------------------------------------------------------------------------------
        // �擪�̎擾
        inline shared getFront() noexcept {
            shared front = shared_from_this();
            while (front->getPrev()) {
                front = front->getPrev();
            }
            return front;
        }

        //-----------------------------------------------------------------------------------------------------
        // �Ō���̎擾
        inline shared getBack() noexcept {
            shared back = shared_from_this();
            while (back->next_) {
                back = back->next_;
            }
            return back;
        }

        //-----------------------------------------------------------------------------------------------------
        // ���E
        // ret... ���̃m�[�h
        inline shared pop() noexcept {
            shared prev = getPrev();
            shared next = getNext();
            if (next) next->prev_ = prev;
            if (prev) prev->next_ = next;
			prev_.reset();
			next_.reset();
            return next;
        }

        //-----------------------------------------------------------------------------------------------------
        // �ǉ�
        inline void pushBack(const shared& add) noexcept {
            shared last = getBack();
            last->next_ = add;
            add->prev_ = last;
        }

        //-----------------------------------------------------------------------------------------------------
        // �T�C�Y�擾
        inline uint32_t getSize() {
			shared obj = getFront();
			for (uint32_t i = 1; ; ++i) {
				obj = obj->getNext();
				if (!obj) return i;
			}
		}

        //-----------------------------------------------------------------------------------------------------
        // ����
        // arg1... ���X�g���̂����ꂩ�̃m�[�h
        // arg2... ��r�֐� ( �����ɍ��v������ true ��Ԃ������_�֐� )
        // ret.... �����ɍ��v�����m�[�h���X�g
        static std::list<shared> Find(shared node, const std::function<bool(shared)>& comp) noexcept {
            std::list<shared> tbl ;
            shared check = node->getFront();
            while (check) {
                if (comp(check)) tbl.emplace_back( check );
                check = check->getNext();
            }
            return tbl;
        }

        //-----------------------------------------------------------------------------------------------------
        // �\�[�g
        // DerivedType [ �h����̌^ ]
        // CompType [ ��r�Ɏg�p����ϐ��̌^ ]
        // arg1... ���X�g���̂����ꂩ�̃m�[�h
        // arg2... �\�[�g�̔�r�Ɏg�p����l��Ԃ������_�֐�
        // ret.... �\�[�g���ʂ̐擪�m�[�h
        // tips... prev �� weak_ptr �ŕێ����Ă���֌W�Ń\�[�g���s��̐擪�m�[�h�͖߂�l�ŏ㏑�����鎖
        template< class DerivedType, class CompType >
		static std::shared_ptr<DerivedType> SortAscending(shared node, const std::function<CompType(shared)>& get_comp_vlue) {
            shared front = node->getFront() ;
            shared back = node->getBack() ;
            return std::static_pointer_cast<DerivedType>(Sort_ascending( front, front, back, get_comp_vlue) );
		}
        template< class DerivedType, class CompType >
        static std::shared_ptr<DerivedType> SortDescending(shared node, const std::function<CompType(shared)>& get_comp_vlue) {
            shared front = node->getFront();
            shared back = node->getBack();
            return std::static_pointer_cast<DerivedType>(Sort_descending( front, front, back, get_comp_vlue));
        }

    };

}

