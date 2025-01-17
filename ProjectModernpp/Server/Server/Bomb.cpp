import Bomb;

Bomb::Bomb(const Point& position, float activationTime)
    : m_position{ position }, m_activationTime{ activationTime }, m_active{ false } {
}

void Bomb::Update(float deltaTime)
{
    if (!m_active) {
        m_activationTime -= deltaTime;
        if (m_activationTime <= 0.0f) {
            m_active = true;
        }
    }
}

bool Bomb::IsActive() const
{
    return m_active;
}

const Point& Bomb::GetPosition() const
{
    return m_position;
}