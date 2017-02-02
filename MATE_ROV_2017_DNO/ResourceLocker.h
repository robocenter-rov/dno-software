#pragma once

enum RESOURCE {
	R_FLASHLIGHT,
	R_BLUETOOTH,

	R_MAX_RESOURCE_ID
};

class ResourceLocker {
private:
	static int _owners[R_MAX_RESOURCE_ID];
	ResourceLocker() {}
public:
	class AutoLocker_t {
	private:
		RESOURCE _resource;
		bool _locking_successfull;
		int _owner;
	public:
		AutoLocker_t(RESOURCE resource);
		bool Lock(int owner);
		int RealOwner() const;
		~AutoLocker_t();
	};

	static void Init();
	static bool Lock(RESOURCE resource, int owner);
	static void Release(RESOURCE resource);
	static bool IsLocked(RESOURCE resource);
	static int GetOwner(RESOURCE resource);
};

