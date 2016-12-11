#include "ResourceLocker.h"
#include "Arduino.h"

int ResourceLocker::_owners[R_MAX_RESOURCE_ID] = {-1};

ResourceLocker::AutoLocker_t::AutoLocker_t(RESOURCE resource, int owner) {
	_resource = resource;
	_owner = owner;
	_locking_successfull = false;
}

bool ResourceLocker::AutoLocker_t::Lock() {
	return (_locking_successfull = ResourceLocker::Lock(_resource, _owner));
}

int ResourceLocker::AutoLocker_t::RealOwner() const {
	return GetOwner(_resource);
}

ResourceLocker::AutoLocker_t::~AutoLocker_t() {
	if (_locking_successfull) {
		Release(_resource);
	}
}

bool ResourceLocker::Lock(RESOURCE resource, int owner) {
#ifdef _DEBUG
	Serial.print("Try lock resource: ");
	Serial.print(int(resource));
	Serial.print(", for ");
	Serial.println(owner);
#endif
	if (IsLocked(resource)) {
#ifdef _DEBUG
		Serial.print("Failed: Resource is locked by: ");
		Serial.println(_owners[resource]);
#endif
		return false;
	} else {
#ifdef _DEBUG
		Serial.println("Successfully locked");
#endif
		_owners[resource] = owner;
		return true;
	}
}

void ResourceLocker::Release(RESOURCE resource) {
#ifdef _DEBUG
	Serial.print("Releasing resource: ");
	Serial.print(int(resource));
	Serial.print(", by ");
	Serial.println(_owners[resource]);
#endif
	_owners[resource] = -1;
}

bool ResourceLocker::IsLocked(RESOURCE resource) {
	return _owners[resource] >= 0;
}

int ResourceLocker::GetOwner(RESOURCE resource) {
	return _owners[resource];
}
