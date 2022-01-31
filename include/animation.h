class Animation
{
private:
    long _runTime = 0;


public:
    explicit Animation();
    ~Animation();

    virtual int Start();
};

Animation::Animation()
{
}

Animation::~Animation()
{
}

int Animation::Start()
{
}
