#include "core_ext_pch.h"
#include "..\..\include\core\ext\PropertyManager.h"
#include "core\GameObject.h"
#include "Package.h"

namespace engine
{

	PropertyManagerPtr PropertyManager::CreateComponent()
	{
		return Package::GetAllocator()->AllocObject<PropertyManager>();
	}
	PropertyManager::PropertyManager(void) : GameObjectComponent(L"PropertyManager")
	{
		m_pCurrentOwner = PropertySetPtr();
	}


	PropertyManager::~PropertyManager(void)
	{
		m_PropSets.clear();
	}
	PropertySetPtr PropertyManager::FindProperty(const std::wstring& name)
	{
		for(size_t i = 0; i < m_PropSets.size(); ++i)
		{
			if(m_PropSets[i]->getName() == name)
			{
				return m_PropSets[i];
			}
		}
		return PropertySetPtr();
	}
	PropertySetPtr PropertyManager::Begin(const std::wstring& name)
	{
		PropertySetPtr pSet = Package::GetAllocator()->AllocObject<PropertySet>(name);

		m_PropSets.push_back(pSet);

		m_pCurrentOwner = pSet;

		return pSet;

	}
	void PropertyManager::AddProperty(boost::shared_ptr<Property> pProp)
	{
		m_pCurrentOwner->addProperty(pProp);
	}

	void PropertyManager::End()
	{
		m_pCurrentOwner = PropertySetPtr();
	}
	bool PropertyManager::OnAttach()
	{
		m_pCurrentOwner = PropertySetPtr();


		Begin(L"General");
		{

			//RegisterProperty<std::wstring>(L"Name", 
			//	//Property_T<std::wstring>::Setter_T(),	
			//	boost::bind(&GameObject::SetName, m_pObject.get(), _1),
			//	boost::bind(&GameObject::GetName, m_pObject.get()));

			RegisterProperty<std::wstring, GameObject>(m_pObject.get(), 
				L"Name", 
				&GameObject::GetName,
				&GameObject::SetName);

			RegisterProperty<math::Matrix44, GameObject>(m_pObject.get(),
				L"Transform", 
				&GameObject::GetLocalTransform,
				&GameObject::SetLocalTransform);
		}
		End();

		return true;
	}
	void PropertyManager::OnDetach()
	{
	}
	int PropertyManager::GetPropertySetCount()
	{
		return m_PropSets.size();
	}
	PropertySetPtr PropertyManager::GetPropertySet(int index)
	{
		return m_PropSets[index];
	}
}