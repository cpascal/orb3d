#pragma once

#include <boost/enable_shared_from_this.hpp>

namespace engine
{
	class EXPORT_CLASS Node: public boost::enable_shared_from_this<Node>
	{
	public:
		Node(void);
		virtual ~Node(void);

		// local transform operation
		void					SetLocalTransform(const math::Matrix44& Local);
		const math::Matrix44&	GetLocalTransform() const;

		// translation
		void					SetTranslation(const math::Vector3& t);
		void					SetTranslation(Real x, Real y, Real z);
		
		void					Translate(const math::Vector3& t);
		void					Translate(Real x, Real y, Real z);

		math::Vector3			GetTranslation() const;

		// rotation
		void					SetRotationAxis(const math::Vector3& axis, Real rad);
		void					SetRotationAxisX(Real rad);
		void					SetRotationAxisY(Real rad);
		void					SetRotationAxisZ(Real rad);
		void					SetRotation(const math::Quat& r);
		void					SetRotation(const math::Matrix44& rot);

		void					Rotate(const math::Quat& r);
		void					Rotate(const math::Matrix44& rot);
		void					RotateAxis(const math::Vector3& axis, Real rad);
		void					RotateAxisX(Real rad);
		void					RotateAxisY(Real rad);
		void					RotateAxisZ(Real rad);

		math::Quat				GetRotation() const;

		// scale
		void					SetScale(const math::Vector3& s);
		void					SetScale(Real x, Real y, Real z);
		math::Vector3			GetScale() const;
		void					SetUnitformScale(Real s);
        
		void					LookAt(NodePtr pNode);
		void					LookAt(const math::Vector3& at);

		// parent/child operation
		void					LinkTo(NodePtr pParent);
		void					UnLink();
		NodePtr					GetFirstChild();
		
		NodePtr					GetParent();

		NodePtr					GetPrevNode();
		NodePtr					GetNextNode();
		void					SetPrevNode(NodePtr pNode);
		void					SetNextNode(NodePtr pNode);
		
		void					ClearChildren();

		// world transform operation
		const math::Matrix44&	GetWorldTransform();

	private:
		void					UpdateWorldTransform();
	private:
		math::Matrix44			m_LocalTransform;
		math::Matrix44			m_WorldTransform;

		NodePtr					m_pParent;
		NodePtr					m_pFirstChild;

		NodePtr					m_pNext;
		NodePtr					m_pPrev;
	};
}