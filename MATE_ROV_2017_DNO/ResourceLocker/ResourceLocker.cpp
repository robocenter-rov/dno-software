#include "../Utils/Debug.h"
#include "ResourceLocker.h"
#include "Arduino.h"

int ResourceLocker::_owners[R_MAX_RESOURCE_ID];

ResourceLocker::AutoLocker_t::AutoLocker_t(RESOURCE resource) {
	_resource = resource;
	_locking_successfull = false;
}

bool ResourceLocker::AutoLocker_t::Lock(int owner) {
	_owner = owner;
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

void ResourceLocker::Init() {
	for (int i = 0; i < R_MAX_RESOURCE_ID; i++) {
		_owners[i] = -1;
	}
}

bool ResourceLocker::Lock(RESOURCE resource, int owner) {
	LOG("Try lock resource: ");
	LOG(int(resource));
	LOG(", for ");
	LOGLN(owner);

	if (IsLocked(resource)) {

		LOG("Failed: Resource is locked by: ");
		LOGLN(_owners[resource]);

		return false;
	} else {

		LOGLN("Successfully locked");

		_owners[resource] = owner;
		return true;
	}
}

void ResourceLocker::Release(RESOURCE resource) {
	LOG("Releasing resource: ");
	LOG(int(resource));
	LOG(", by ");
	LOGLN(_owners[resource]);

	_owners[resource] = -1;
}

bool ResourceLocker::IsLocked(RESOURCE resource) {
	return _owners[resource] >= 0;
}

int ResourceLocker::GetOwner(RESOURCE resource) {
	return _owners[resource];
}
